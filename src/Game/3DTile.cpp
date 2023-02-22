#include "3DTile.h"
//-----------------------------------------------------------------------------
ShaderProgram shader;
Uniform uniformWorldMatrix;
Uniform uniformViewMatrix;
Uniform uniformProjectionMatrix;
Uniform uniformLight;

Texture2D defaultTexture;
Model wallModel;
Model floorModel[7];
Model ceilModel;

https://sketchfab.com/3d-models/low-poly-nature-pack-by-rgsdev-4b7e5b2130384655a7ccdd7e7b711836
https://github.com/voithos/quarkGL/tree/master/examples
//-----------------------------------------------------------------------------
bool Tile3DManager::Create()
{
	constexpr const char* vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexTexCoord;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fragmentColor;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	gl_Position   = uProjection * uView * uWorld * vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
	Normal        = vertexNormal;
	TexCoord      = vertexTexCoord;
}
)";
	constexpr const char* fragmentShaderText = R"(
#version 330 core

in vec3 fragmentColor;
in vec3 Normal;
in vec2 TexCoord;

struct DirectionalLight
{
	float Ambient, Diffuse;
	vec3 Direction;
};
uniform DirectionalLight Light;

uniform sampler2D Texture;

out vec4 outColor;

void main()
{
	outColor = texture(Texture, TexCoord) * vec4(fragmentColor, 1.0);

	float NdotLD = max(dot(Light.Direction, normalize(Normal)), 0.0); // ламберт
	outColor.rgb *= Light.Ambient + Light.Diffuse * NdotLD;
	//float attenuation = saturate(1.0 - DistanceToLight / LightRadius);
	//frag_Color.rgb *= Light.Ambient + Light.Diffuse * NdotLD * attenuation;
}
)";

	if( !shader.CreateFromMemories(vertexShaderText, fragmentShaderText) )
		return false;

	uniformWorldMatrix = shader["uWorld"];
	uniformViewMatrix = shader["uView"];
	uniformProjectionMatrix = shader["uProjection"];
	uniformLight = shader["uniformLight"];


	Texture2DInfo texInfo;
	texInfo.mipmap = false;
	texInfo.minFilter = TextureMinFilter::Nearest;
	texInfo.magFilter = TextureMagFilter::Nearest;
	if( !defaultTexture.Create("../data/textures/tile.png", texInfo) )
		return false;

	// wall
	{
		std::vector<Mesh> meshData(1);
		meshData[0].vertices = {
			{ {-0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ { 0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ { 0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ {-0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// front

			{ { 0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ {-0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ {-0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ { 0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// back

			{ {-0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ { 0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ { 0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ {-0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// top

			{ { 0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ {-0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ {-0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ { 0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// bottom

			{ {-0.5f, 0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ {-0.5f,-0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ {-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// left

			{ { 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ { 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ { 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ { 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// right
		};

		meshData[0].indices = {
			0, 3, 1,  1, 3, 2, // front
			4, 7, 5,  5, 7, 6, // back
			8,11, 9,  9,11,10, // top
			12,15,13, 13,15,14, // bottom
			16,19,17, 17,19,18, // left
			20,23,21, 21,23,22  // right
		};

		meshData[0].material = { .diffuseTexture = &defaultTexture };
		wallModel.Create(std::move(meshData));
	}

	// floor
	{
		for( int i = 1; i < 8; i++ )
		{
			floorModel[i - 1].Create(("../data/mesh/tilesFloor/tile" + std::to_string(i) + ".obj").c_str());
			floorModel[i - 1].SetMaterial({ .diffuseTexture = &defaultTexture });
		}
	}


	return true;
}
//-----------------------------------------------------------------------------
void Tile3DManager::Destroy()
{
	defaultTexture.Destroy();
	shader.Destroy();
	wallModel.Destroy();
}
//-----------------------------------------------------------------------------
void Tile3DManager::BeginDraw(const Matrix4& proj, const Matrix4& view)
{
	shader.Bind();
	uniformViewMatrix = view;
	uniformProjectionMatrix = proj;

	shader.SetUniform(shader.GetUniformLocation("Light.Ambient"), 0.333333f);
	shader.SetUniform(shader.GetUniformLocation("Light.Diffuse"), 0.666666f);
	Vector3 LightDirection = Vector3(0.0f, 0.5f, -1.0f);
	shader.SetUniform(shader.GetUniformLocation("Light.Direction"), LightDirection);
}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawWall(const Vector3& position)
{
	Matrix4 world = Matrix4::Translate(Matrix4::Identity, position);
	uniformWorldMatrix = world;
	wallModel.Draw();
}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawFloor(const Vector3& position)
{
	Matrix4 world = Matrix4::Translate(Matrix4::Identity, position);
	uniformWorldMatrix = world;
	floorModel[3].Draw();
}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawCeil(const Vector3& position)
{

}
//-----------------------------------------------------------------------------