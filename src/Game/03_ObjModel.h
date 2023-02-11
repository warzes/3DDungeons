#pragma once

/*
- загрузка меша с файла
- создание меша с заданием вершин
- работа с камерой
*/

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
out vec2 TexCoord;

void main()
{
	gl_Position   = uProjection * uView * uWorld * vec4(vertexPosition, 1.0);
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

ShaderProgram shader;
int uniformWorldMatrix;
int uniformViewMatrix;
int uniformProjectionMatrix;
Texture2D texture;
Model model;
Model customModel;

FlyingCamera cam;

void ExampleInit()
{
	shader.CreateFromMemories(vertexShaderText, fragmentShaderText);
	uniformWorldMatrix = shader.GetUniformLocation("uWorld");
	uniformViewMatrix = shader.GetUniformLocation("uView");
	uniformProjectionMatrix = shader.GetUniformLocation("uProjection");

	texture.Create("../data/textures/stone03b.jpg");

	model.Create("../data/mesh/rock.obj", "../data/mesh/");
	//model.SetMaterial({ .diffuseTexture = &texture }); // если материала нет

	// create custom model
	{
		std::vector<Mesh> meshData(1);
		meshData[0].vertices = {
			{ {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
			{ { 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ { 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
		};
		meshData[0].indices = { 0, 1, 2, 2, 3, 0 };
		meshData[0].material = { .diffuseTexture = &texture };
		customModel.Create(std::move(meshData));
	}

	cam.Look({ 0.0f, 3.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });

	//SetMouseVisible(false);
}

void ExampleClose()
{
	texture.Destroy();
	shader.Destroy();
	model.Destroy();
	customModel.Destroy();
}

void ExampleFrame()
{
	auto deltaCursor = GetCursorDelta();
	if ( deltaCursor.x != 0 || deltaCursor.y != 0)
	{
		if (IsMouseButtonDown(MouseButton::Right))
			cam.OnMouseMove(deltaCursor.x, deltaCursor.y);
	}

	short Keys = 0x0000;
	if (IsKeyDown('W')) Keys |= CAMERA_KEY_W;
	if (IsKeyDown('S')) Keys |= CAMERA_KEY_S;
	if (IsKeyDown('A')) Keys |= CAMERA_KEY_A;
	if (IsKeyDown('D')) Keys |= CAMERA_KEY_D;
	if (IsKeyDown('R')) Keys |= CAMERA_KEY_R;
	if (IsKeyDown('F')) Keys |= CAMERA_KEY_F;
	if (IsKeyDown('Q')) Keys |= CAMERA_KEY_Q;
	if (IsKeyDown('E')) Keys |= CAMERA_KEY_E;
	if (IsKeyDown('C')) Keys |= CAMERA_KEY_E;

	if (IsKeyDown(0x20/*VK_SPACE*/)) Keys |= CAMERA_KEY_SPACE;
	if (IsKeyDown(0x10/*VK_SHIFT*/)) Keys |= CAMERA_KEY_SHIFT;
	if (IsKeyDown(0x11/*VK_CONTROL*/)) Keys |= CAMERA_KEY_CONTROL;

	Vector3 Movement;
	bool MoveCamera = cam.OnKeys(Keys, 0.01f, Movement);
	if (MoveCamera) cam.Move(Movement);

	Matrix4 view = cam.GetViewMatrix();
	Matrix4 perpective = Perspective(45.0f, GetWindowAspectRatio(), 0.01f, 1000.f);
	Matrix4 world1;
	Matrix4 world2 = Matrix4Translate({ 3.0f, 0.2f, 0.4f });
	Matrix4 PV = perpective * view;

	Matrix4 lk2 = LookAt(Vector3(0.0f, 3.0f, -6.0f), Vector3(0.0f), Vector3(0.0f, 1.0f, 0.0f));

	shader.Bind();
	shader.SetUniform(uniformViewMatrix, view);
	shader.SetUniform(uniformProjectionMatrix, perpective);

	shader.SetUniform(uniformWorldMatrix, world1);
	model.Draw();

	shader.SetUniform(uniformWorldMatrix, world2);
	customModel.Draw();

	DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, { -10.0f, 2.0f, 5.0f }, RED);

	DebugDraw::Flush(perpective * view);
}