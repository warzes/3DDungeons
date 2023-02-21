#include "DCGameApp.h"
#include "3DTile.h"

//https://www.youtube.com/watch?v=Vr-Fsd6M5Tk
//https://www.youtube.com/watch?v=VjuyfBaryu8&t=3s

//вынести в отдельный проект
//
//куб с индексбуфером есть тут - TempCubeVertices
//
//диздок
//идея такая - это пошаговый данжшен кравлер от первого лица
//игрок играет пати.
//монстры видны. при приближении атакуют
//ходы по очереди
//примерно как в Legends of Amberland The Forgotten Crown
//
//мир состоит из кубов
//при этом у куба есть высота чтобы получить такой эффект - https://mudgate.itch.io/mudgate
//столкновения такие - при движении проверяем высоту клетки, если шаг допустим - подымаемся (с модом высоты) или стопоримся
//позже продумать скаты и спуски. Честная коллизия не нужна
//
//текстуры для старта взять отсюда - https://forum.zdoom.org/viewtopic.php?t=63994

FlyingCamera cam;

bool GameAppInit()
{
	if( !Tile3DManager::Create() )
		return false;

	cam.Look({ 0.0f, 3.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	//SetMouseVisible(false);

	return true;
}

void GameAppClose()
{
	Tile3DManager::Destroy();
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
	Tile3DManager::BeginDraw(perpective, view);

	for( size_t x = 0; x < 100; x++ )
	{
		for( size_t y = 0; y < 100; y++ )
		{
			for( size_t z = 0; z < 10; z++ )
			{
				Vector3 pos;
				pos.x = x;
				pos.y = z;
				pos.z = y;
				Tile3DManager::DrawWall(pos);
			}
		}
	}

	//DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, { -10.0f, 2.0f, 0.0f }, RED);
	//DebugDraw::Flush(perpective * view);

	DebugText::Begin();
	DebugText::SetForeground({ 255, 255, 0, 255 });
	DebugText::SetBackground({ 100, 120, 255, 255 });
	DebugText::Print(1, 1, "Hello");
	DebugText::Flush();
}