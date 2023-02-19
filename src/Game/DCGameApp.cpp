#include "DCGameApp.h"

//https://www.youtube.com/watch?v=Vr-Fsd6M5Tk
//https://www.youtube.com/watch?v=VjuyfBaryu8&t=3s

//вынести в отдельный проект
//
//куб с индексбуфером есть тут - TempCubeVertices
//
//диздок
//иде€ така€ - это пошаговый данжшен кравлер от первого лица
//игрок играет пати.
//монстры видны. при приближении атакуют
//ходы по очереди
//примерно как в Legends of Amberland The Forgotten Crown
//
//мир состоит из кубов
//при этом у куба есть высота чтобы получить такой эффект - https://mudgate.itch.io/mudgate
//столкновени€ такие - при движении провер€ем высоту клетки, если шаг допустим - подымаемс€ (с модом высоты) или стопоримс€
//позже продумать скаты и спуски. „естна€ коллизи€ не нужна
//
//текстуры дл€ старта вз€ть отсюда - https://forum.zdoom.org/viewtopic.php?t=63994

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

ShaderProgram shader;
int uniformWorldMatrix;
int uniformViewMatrix;
int uniformProjectionMatrix;
int uniformLight;
Texture2D texture;
Model customModel;

FlyingCamera cam;

void GameAppInit()
{
	shader.CreateFromMemories(vertexShaderText, fragmentShaderText);
	uniformWorldMatrix = shader.GetUniformLocation("uWorld");
	uniformViewMatrix = shader.GetUniformLocation("uView");
	uniformProjectionMatrix = shader.GetUniformLocation("uProjection");
	uniformLight = shader.GetUniformLocation("uniformLight");

	texture.Create("../data/textures/tile.png");

	// create custom model
	{
		std::vector<Mesh> meshData(1);
#if 0
		meshData[0].vertices = {
			{ {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
			{ { 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
			{ { 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
			{ {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
		};
		meshData[0].indices = { 0, 1, 2, 2, 3, 0 };
#else
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
#endif
		meshData[0].material = { .diffuseTexture = &texture };
		customModel.Create(std::move(meshData));
	}

	cam.Look({ 0.0f, 3.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });

	//SetMouseVisible(false);
}

void GameAppClose()
{
	texture.Destroy();
	shader.Destroy();
	customModel.Destroy();
}

void GameAppFrame()
{
	auto deltaCursor = GetCursorDelta();
	if( deltaCursor.x != 0 || deltaCursor.y != 0 )
	{
		if( IsMouseButtonDown(MouseButton::Right) )
			cam.OnMouseMove(deltaCursor.x, deltaCursor.y);
	}

	short Keys = 0x0000;
	if( IsKeyDown('W') ) Keys |= CAMERA_KEY_W;
	if( IsKeyDown('S') ) Keys |= CAMERA_KEY_S;
	if( IsKeyDown('A') ) Keys |= CAMERA_KEY_A;
	if( IsKeyDown('D') ) Keys |= CAMERA_KEY_D;
	if( IsKeyDown('R') ) Keys |= CAMERA_KEY_R;
	if( IsKeyDown('F') ) Keys |= CAMERA_KEY_F;
	if( IsKeyDown('Q') ) Keys |= CAMERA_KEY_Q;
	if( IsKeyDown('E') ) Keys |= CAMERA_KEY_E;
	if( IsKeyDown('C') ) Keys |= CAMERA_KEY_E;

	if( IsKeyDown(0x20/*VK_SPACE*/) ) Keys |= CAMERA_KEY_SPACE;
	if( IsKeyDown(0x10/*VK_SHIFT*/) ) Keys |= CAMERA_KEY_SHIFT;
	if( IsKeyDown(0x11/*VK_CONTROL*/) ) Keys |= CAMERA_KEY_CONTROL;

	Vector3 Movement;
	bool MoveCamera = cam.OnKeys(Keys, 0.01f, Movement);
	if( MoveCamera ) cam.Move(Movement);

	Matrix4 view = cam.GetViewMatrix();
	Matrix4 perpective = Matrix4::Perspective(45.0f, GetWindowAspectRatio(), 0.01f, 1000.f);
	Matrix4 world1;
	Matrix4 world2 = Matrix4::Translate(Matrix4::Identity, { 3.0f, 0.2f, 0.4f });

	shader.Bind();
	shader.SetUniform(uniformViewMatrix, view);
	shader.SetUniform(uniformProjectionMatrix, perpective);

	shader.SetUniform(uniformWorldMatrix, world1);

	shader.SetUniform(shader.GetUniformLocation("Light.Ambient"), 0.333333f);
	shader.SetUniform(shader.GetUniformLocation("Light.Diffuse"), 0.666666f);
	Vector3 LightDirection = Vector3(0.0f, 0.0f, 1.0f);
	shader.SetUniform(shader.GetUniformLocation("Light.Direction"), LightDirection);




	customModel.Draw();

	DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, { -10.0f, 2.0f, 0.0f }, RED);

	DebugDraw::Flush(perpective * view);

	DebugText::Begin();
	DebugText::SetForeground({ 255, 255, 0, 255 });
	DebugText::SetBackground({ 100, 120, 255, 255 });
	DebugText::Print(1, 1, "Hello");
	DebugText::Flush();
}