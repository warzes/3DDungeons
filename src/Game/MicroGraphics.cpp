//=============================================================================
// Header
//=============================================================================
#include "MicroGraphics.h"

#if defined(_MSC_VER)
#	pragma warning(push, 0)
#endif // _MSC_VER
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <string>
#include <vector>
#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER
//-----------------------------------------------------------------------------
// MicroEngine
extern void LogError(const std::string& msg);
extern void LogWarning(const std::string& msg);
// MicroAdvance
namespace ResourceCacheSystem
{
	extern Texture2D* LoadTexture2D(const char* fileName, const Texture2DInfo& textureInfo);
}
//-----------------------------------------------------------------------------
namespace std
{
	// магия хеширования из glm, по хорошему изменить
	void hash_combine(size_t& seed, size_t hash)
	{
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
	}

	template<>
	struct hash<Vector2>
	{
		size_t operator()(const Vector2& v) const
		{
			size_t seed = 0;
			hash<float> hasher;
			hash_combine(seed, hasher(v.x));
			hash_combine(seed, hasher(v.y));
			return seed;
		}
	};

	template<>
	struct hash<Vector3>
	{
		size_t operator()(const Vector3& v) const
		{
			size_t seed = 0;
			hash<float> hasher;
			hash_combine(seed, hasher(v.x));
			hash_combine(seed, hasher(v.y));
			hash_combine(seed, hasher(v.z));
			return seed;
		}
	};

	template <>
	struct hash<VertexMesh>
	{
		size_t operator()(const VertexMesh& vertex) const
		{
			return ((hash<Vector3>()(vertex.position) ^ (hash<Vector2>()(vertex.texCoord) << 1)) >> 1);
		}
	};
} // namespace std
//-----------------------------------------------------------------------------
//=============================================================================
// Model
//=============================================================================
//-----------------------------------------------------------------------------
std::vector<Vector3> Mesh::GetTriangles() const
{
	std::vector<Vector3> v;
	// востановление треугольников по индексному буферу
	for( size_t i = 0; i < indices.size(); i++ ) 
		v.push_back(vertices[indices[i]].position);
	return v;
}
//-----------------------------------------------------------------------------
bool Model::Create(const char* fileName, const char* pathMaterialFiles)
{
	Destroy();
	bool success = false;
	if( std::string(fileName).find(".obj") != std::string::npos )
	{
		success = loadObjFile(fileName, pathMaterialFiles);
	}

	return success;
}
//-----------------------------------------------------------------------------
bool Model::Create(std::vector<Mesh>&& meshes)
{
	Destroy();
	m_subMeshes = std::move(meshes);
	return createBuffer();
}
//-----------------------------------------------------------------------------
void Model::Destroy()
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i].vertices.clear();
		m_subMeshes[i].indices.clear();

		m_subMeshes[i].vertexBuffer.Destroy();
		m_subMeshes[i].indexBuffer.Destroy();
		m_subMeshes[i].vao.Destroy();
	}
	m_subMeshes.clear();
}
//-----------------------------------------------------------------------------
void Model::SetMaterial(const Material& material)
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i].material = material;
	}
}
//-----------------------------------------------------------------------------
void Model::Draw()
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		if (m_subMeshes[i].vao.IsValid())
		{
			const Texture2D* diffuseTexture = m_subMeshes[i].material.diffuseTexture;
			if (diffuseTexture && diffuseTexture->IsValid())
				diffuseTexture->Bind(0);
			m_subMeshes[i].vao.Draw(PrimitiveDraw::Triangles);
		}
	}
}
//-----------------------------------------------------------------------------
std::vector<Vector3> Model::GetTriangles() const
{
	std::vector<Vector3> v;
	for( size_t i = 0; i < m_subMeshes.size(); i++ )
	{
		auto subV = m_subMeshes[i].GetTriangles();
		// https://www.techiedelight.com/ru/concatenate-two-vectors-cpp/
		std::move(subV.begin(), subV.end(), std::back_inserter(v));
	}
	return v;
}
//-----------------------------------------------------------------------------
bool Model::loadObjFile(const char* fileName, const char* pathMaterialFiles)
{
	tinyobj::ObjReaderConfig readerConfig;
	readerConfig.mtl_search_path = pathMaterialFiles; // Path to material files

	tinyobj::ObjReader reader;
	if (!reader.ParseFromFile(fileName, readerConfig))
	{
		if (!reader.Error().empty())
			LogError("TinyObjReader: " + reader.Error());
		return false;
	}
	if (!reader.Warning().empty())
		LogWarning("TinyObjReader: " + reader.Warning());

	auto& attributes = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	const bool isFindMaterials = !materials.empty();

	std::vector<Mesh> tempMesh(materials.size());
	std::vector<std::unordered_map<VertexMesh, uint32_t>> uniqueVertices(materials.size());
	if (tempMesh.empty())
	{
		tempMesh.resize(1);
		uniqueVertices.resize(1);
	}

	// Loop over shapes
	for (size_t shapeId = 0; shapeId < shapes.size(); shapeId++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t faceId = 0; faceId < shapes[shapeId].mesh.num_face_vertices.size(); faceId++)
		{
			const size_t fv = static_cast<size_t>(shapes[shapeId].mesh.num_face_vertices[faceId]);

			// per-face material
			int materialId = shapes[shapeId].mesh.material_ids[faceId];
			if (materialId < 0) materialId = 0;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				const tinyobj::index_t idx = shapes[shapeId].mesh.indices[index_offset + v];

				// vertex position
				const tinyobj::real_t vx = attributes.vertices[3 * size_t(idx.vertex_index) + 0];
				const tinyobj::real_t vy = attributes.vertices[3 * size_t(idx.vertex_index) + 1];
				const tinyobj::real_t vz = attributes.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				tinyobj::real_t nx = 0.0f;
				tinyobj::real_t ny = 0.0f;
				tinyobj::real_t nz = 0.0f;
				if (idx.normal_index >= 0)
				{
					nx = attributes.normals[3 * size_t(idx.normal_index) + 0];
					ny = attributes.normals[3 * size_t(idx.normal_index) + 1];
					nz = attributes.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				tinyobj::real_t tx = 0;
				tinyobj::real_t ty = 0;
				if (idx.texcoord_index >= 0)
				{
					tx = attributes.texcoords[2 * size_t(idx.texcoord_index) + 0];
					ty = attributes.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// vertex colors
				const tinyobj::real_t r = attributes.colors[3 * size_t(idx.vertex_index) + 0];
				const tinyobj::real_t g = attributes.colors[3 * size_t(idx.vertex_index) + 1];
				const tinyobj::real_t b = attributes.colors[3 * size_t(idx.vertex_index) + 2];

				VertexMesh vertex;
				vertex.position = { vx, vy, vz };
				vertex.normal = {nx, ny, nz};
				vertex.color = { r, g, b };
				vertex.texCoord = { tx,ty };

				if (uniqueVertices[materialId].count(vertex) == 0)
				{
					uniqueVertices[materialId][vertex] = static_cast<uint32_t>(tempMesh[materialId].vertices.size());
					tempMesh[materialId].vertices.emplace_back(vertex);
				}

				tempMesh[materialId].indices.emplace_back(uniqueVertices[materialId][vertex]);
			}
			index_offset += fv;
		}
	}

	// load materials
	bool isFindToTransparent = false;
	if (isFindMaterials)
	{
		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i].diffuse_texname.empty()) continue;

			std::string diffuseMap = pathMaterialFiles + materials[i].diffuse_texname;
			tempMesh[i].material.diffuseTexture = ResourceCacheSystem::LoadTexture2D(diffuseMap.c_str(), {});
			if (!isFindToTransparent && tempMesh[i].material.diffuseTexture)
				isFindToTransparent = tempMesh[i].material.diffuseTexture->isTransparent;
		}
	}

	// сортировка по прозрачности
	if (isFindToTransparent)
	{
		std::vector<Mesh> tempMesh2;

		// TODO: медленно, оптимизировать

		// сначала непрозрачное
		for (int i = 0; i < tempMesh.size(); i++)
		{
			if (!tempMesh[i].material.diffuseTexture)
				tempMesh2.push_back(tempMesh[i]);
			else if (!tempMesh[i].material.diffuseTexture->isTransparent)
				tempMesh2.push_back(tempMesh[i]);
		}
		// теперь прозрачное
		for (int i = 0; i < tempMesh.size(); i++)
		{
			if (tempMesh[i].material.diffuseTexture->isTransparent)
				tempMesh2.push_back(tempMesh[i]);
		}

		m_subMeshes = std::move(tempMesh2);
	}
	else
		m_subMeshes = std::move(tempMesh);

	return createBuffer();
}
//-----------------------------------------------------------------------------
bool Model::createBuffer()
{
	// формат вершин
	const std::vector<VertexAttribute> formatVertex =
	{
		{.size = 3, .normalized = false, .stride = sizeof(VertexMesh), .offset = (void*)offsetof(VertexMesh, position)},
		{.size = 3, .normalized = false, .stride = sizeof(VertexMesh), .offset = (void*)offsetof(VertexMesh, normal)},
		{.size = 3, .normalized = false, .stride = sizeof(VertexMesh), .offset = (void*)offsetof(VertexMesh, color)},
		{.size = 2, .normalized = false, .stride = sizeof(VertexMesh), .offset = (void*)offsetof(VertexMesh, texCoord)}
	};

	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		if (!m_subMeshes[i].vertexBuffer.Create(RenderResourceUsage::Static, m_subMeshes[i].vertices.size(), sizeof(m_subMeshes[i].vertices[0]), m_subMeshes[i].vertices.data()))
		{
			LogError("VertexBuffer create failed!");
			Destroy();
			return false;
		}
		if (!m_subMeshes[i].indexBuffer.Create(RenderResourceUsage::Static, m_subMeshes[i].indices.size(), sizeof(uint32_t), m_subMeshes[i].indices.data()))
		{
			LogError("IndexBuffer create failed!");
			Destroy();
			return false;
		}

		if (!m_subMeshes[i].vao.Create(&m_subMeshes[i].vertexBuffer, &m_subMeshes[i].indexBuffer, formatVertex))
		{
			LogError("VAO create failed!");
			Destroy();
			return false;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Camera
//=============================================================================
//-----------------------------------------------------------------------------
void ICamera::Look(const Vector3& Position, const Vector3& Reference)
{
	position = Position;
	z = (Reference - Position).GetNormalize();
	x = CrossProduct(Vector3::Up, z).GetNormalize();
	y = CrossProduct(z, x);

	calculateViewMatrix();
}
//-----------------------------------------------------------------------------
void ICamera::Move(const Vector3& Movement)
{
	position += Movement;
	calculateViewMatrix();
}
//-----------------------------------------------------------------------------
void ICamera::SetPosition(const Vector3& Movement)
{
	position = Movement;
	calculateViewMatrix();
}
//-----------------------------------------------------------------------------
void ICamera::calculateViewMatrix()
{
	// TODO: улучшить
	m_viewMatrix[0] = { x.x, y.x, z.x, 0.0f };
	m_viewMatrix[1] = { x.y, y.y, z.y, 0.0f };
	m_viewMatrix[2] = { x.z, y.z, z.z, 0.0f };
	m_viewMatrix[3] = { -DotProduct(x, position), -DotProduct(y, position), -DotProduct(z, position), 1.0f };
}
//-----------------------------------------------------------------------------
bool FlyingCamera::OnKeys(short Keys, float FrameTime, Vector3& Movement)
{
	float Speed = m_speed;
	if (Keys & CAMERA_KEY_SHIFT) Speed *= 2.0f;
	if (Keys & CAMERA_KEY_CONTROL) Speed *= 0.5f;

	const float Distance = Speed * FrameTime;

	Vector3 Up = y * Distance;
	Vector3 Right = x * Distance;
	Vector3 Forward = z * Distance;

	if (Keys & CAMERA_KEY_W) Movement += Forward;
	if (Keys & CAMERA_KEY_S) Movement -= Forward;
	if (Keys & CAMERA_KEY_A) Movement -= Right;
	if (Keys & CAMERA_KEY_D) Movement += Right;
	if (Keys & CAMERA_KEY_R) Movement += Up;
	if (Keys & CAMERA_KEY_F) Movement -= Up;

	return Keys & 0x3F ? true : false;
}
//-----------------------------------------------------------------------------
void FlyingCamera::OnMouseMove(int dx, int dy)
{
	if (dx != 0)
	{
		const float DeltaX = (float)dx * m_sensitivity;

		x = Rotate(x, DeltaX, Vector3::Up);
		y = Rotate(y, DeltaX, Vector3::Up);
		z = Rotate(z, DeltaX, Vector3::Up);
	}

	if (dy != 0)
	{
		const float DeltaY = (float)dy * m_sensitivity;

		y = Rotate(y, DeltaY, x);
		z = Rotate(z, DeltaY, x);

		if (y.y < 0.0f)
		{
			z = Vector3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			y = CrossProduct(z, x);
		}
	}

	calculateViewMatrix();
}
//-----------------------------------------------------------------------------