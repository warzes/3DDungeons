#pragma once

//=============================================================================
// Header
//=============================================================================

#include <vector>

#include "MicroMath.h"
#include "MicroMathOld.h"
#include "MicroRender.h"

class Texture2D;

//=============================================================================
// Material
//=============================================================================
class Material
{
public:
	Texture2D* diffuseTexture = nullptr;

	Vector3Old ambientColor = { 1.0f };
	Vector3Old diffuseColor = { 1.0f };
	Vector3Old specularColor = { 1.0f };
	float shininess = 1.0f;
};

//=============================================================================
// Mesh
//=============================================================================
struct VertexMesh
{
	bool operator==(const VertexMesh& v) const { return position == v.position && normal == v.normal && color == v.color && texCoord == v.texCoord; }

	Vector3Old position;
	Vector3Old normal;
	Vector3Old color;
	Vector2 texCoord;
};

class Mesh
{
public:
	std::vector<VertexMesh> vertices;
	std::vector<uint32_t> indices;
	Material material;

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	VertexArrayBuffer vao;
};

//=============================================================================
// Model
//=============================================================================
class Model
{
public:
	bool Create(const char* fileName, const char* pathMaterialFiles = "./");
	bool Create(std::vector<Mesh>&& meshes);
	void Destroy();

	void SetMaterial(const Material& material);

	void Draw();

	bool IsValid() const
	{
		if (m_subMeshes.size() > 0)
			return m_subMeshes[0].vertexBuffer.IsValid() && m_subMeshes[0].indexBuffer.IsValid() && m_subMeshes[0].vao.IsValid();
		return false;
	}

private:
	bool loadObjFile(const char* fileName, const char* pathMaterialFiles = "./");
	bool createBuffer();
	std::vector<Mesh> m_subMeshes;
};

//=============================================================================
// Camera
//=============================================================================
#define CAMERA_KEY_W 0x0001
#define CAMERA_KEY_S 0x0002
#define CAMERA_KEY_A 0x0004
#define CAMERA_KEY_D 0x0008
#define CAMERA_KEY_R 0x0010
#define CAMERA_KEY_F 0x0020
#define CAMERA_KEY_Q 0x0040
#define CAMERA_KEY_E 0x0080
#define CAMERA_KEY_C 0x0100
#define CAMERA_KEY_SPACE 0x0200
#define CAMERA_KEY_SHIFT 0x0400
#define CAMERA_KEY_CONTROL 0x0800

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual void Look(const Vector3Old& Position, const Vector3Old& Reference);
	virtual void Move(const Vector3Old& Movement);
	virtual bool OnKeys(short Keys, float FrameTime, Vector3Old& Movement) = 0;
	virtual void OnMouseMove(int dx, int dy) = 0;

	const Matrix4Old& GetViewMatrix() const { return m_viewMatrix; }

	Vector3Old x = Vector3Old::Right;
	Vector3Old y = Vector3Old::Up;
	Vector3Old z = Vector3Old::Forward;
	Vector3Old position = { 0.0f };

protected:
	virtual void calculateViewMatrix();

	Matrix4Old m_viewMatrix;

	float m_speed = 2.5f;
	float m_sensitivity = 0.025f;
};

class FlyingCamera : public ICamera
{
public:
	bool OnKeys(short Keys, float FrameTime, Vector3Old& Movement);
	void OnMouseMove(int dx, int dy);
};