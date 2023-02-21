#include "3DTile.h"
//-----------------------------------------------------------------------------
ShaderProgram shader;
int uniformWorldMatrix;
int uniformViewMatrix;
int uniformProjectionMatrix;
int uniformLight;

Texture2D defaultTexture;
Model wallModel;
Model floorModel;
Model ceilModel;
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

	uniformWorldMatrix = shader.GetUniformLocation("uWorld");
	uniformViewMatrix = shader.GetUniformLocation("uView");
	uniformProjectionMatrix = shader.GetUniformLocation("uProjection");
	uniformLight = shader.GetUniformLocation("uniformLight");


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
	shader.SetUniform(uniformViewMatrix, view);
	shader.SetUniform(uniformProjectionMatrix, proj);


	shader.SetUniform(shader.GetUniformLocation("Light.Ambient"), 0.333333f);
	shader.SetUniform(shader.GetUniformLocation("Light.Diffuse"), 0.666666f);
	Vector3 LightDirection = Vector3(0.0f, 0.0f, 1.0f);
	shader.SetUniform(shader.GetUniformLocation("Light.Direction"), LightDirection);


}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawWall(const Vector3& position)
{
	Matrix4 world = Matrix4::Translate(Matrix4::Identity, position);
	shader.SetUniform(uniformWorldMatrix, world);
	wallModel.Draw();
}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawFloor(const Vector3& position)
{

}
//-----------------------------------------------------------------------------
void Tile3DManager::DrawCeil(const Vector3& position)
{

}
//-----------------------------------------------------------------------------