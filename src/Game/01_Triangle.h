#pragma once

std::string vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 projectionMatrix;

out vec3 fragmentColor;

void main()
{
	gl_Position   = projectionMatrix * vec4(position, 1.0);
	fragmentColor = color;
}
)";

std::string fragmentShaderText = R"(
#version 330 core

in vec3 fragmentColor;
out vec4 color;

void main()
{
	color = vec4(fragmentColor, 1.0);
}
)";

struct testVertex
{
	Vector3 pos;
	Vector3 color;
};

testVertex vert[3] =
{
	{{-1.0f, -1.0f, -4.0f}, {1.0f, 0.0f, 0.0f}},
	{{ 1.0f, -1.0f, -4.0f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.0f,  1.0f, -4.0f}, {0.0f, 0.0f, 1.0f}}
};

ShaderProgram shader;
int uniformProjectionMatrix;
VertexBuffer vb;
VertexArrayBuffer vao;

void ExampleInit()
{
	shader.CreateFromMemories(vertexShaderText, fragmentShaderText);
	uniformProjectionMatrix = shader.GetUniformLocation("projectionMatrix");

	vb.Create(RenderResourceUsage::Static, 3, sizeof(testVertex), vert);
	vao.Create(&vb, nullptr, &shader);
}

void ExampleClose()
{
	shader.Destroy();
	vao.Destroy();
	vb.Destroy();
}

void ExampleFrame()
{
	shader.Bind();
	Matrix4 mat = Matrix4::Perspective(45.0f * DEG2RAD, GetWindowAspectRatio(), 0.01f, 1000.f);
	shader.SetUniform(uniformProjectionMatrix, mat);
	vao.Draw();
}