#pragma once

constexpr const char* vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 projectionMatrix;

out vec3 fragmentColor;
out vec2 TexCoord;

void main()
{
	gl_Position   = projectionMatrix * vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
	TexCoord      = vertexTexCoord;
}
)";

constexpr const char* fragmentShaderText = R"(
#version 330 core

in vec3 fragmentColor;
in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 outColor;

void main()
{
	outColor = texture(Texture, TexCoord) * vec4(fragmentColor, 1.0);
}
)";

struct testVertex
{
	Vector3 pos;
	Vector3 color;
	Vector2 texCoord;
};

testVertex vert[] =
{
	{{-1.0f, -1.0f, 4.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-1.0f,  1.0f, 4.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 1.0f,  1.0f, 4.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
	{{ 1.0f, -1.0f, 4.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};
int index[] =
{
	0, 1, 2,
	2, 3, 0
};

ShaderProgram shader;
int uniformProjectionMatrix;
VertexBuffer vb;
IndexBuffer ib;
VertexArrayBuffer vao;
Texture2D texture;

void ExampleInit()
{
	shader.CreateFromMemories(vertexShaderText, fragmentShaderText);
	uniformProjectionMatrix = shader.GetUniformLocation("projectionMatrix");

	vb.Create(RenderResourceUsage::Static, Countof(vert), sizeof(testVertex), vert);
	ib.Create(RenderResourceUsage::Static, Countof(index), sizeof(int), index);
	vao.Create(&vb, &ib, &shader);

	texture.Create("../data/textures/stone03b.jpg");
}

void ExampleClose()
{
	texture.Destroy();
	shader.Destroy();
	vao.Destroy();
	vb.Destroy();
	ib.Destroy();
}

void ExampleFrame()
{
	texture.Bind();
	shader.Bind();
	Matrix4 mat = Matrix4::Perspective(45.0f * DEG2RAD, GetWindowAspectRatio(), 0.01f, 1000.f);
	shader.SetUniform(uniformProjectionMatrix, mat);
	vao.Draw();
}