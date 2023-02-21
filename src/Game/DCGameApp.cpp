#include "DCGameApp.h"
#include "3DTile.h"
#include "PlayerCamera.h"

//https://www.youtube.com/watch?v=Vr-Fsd6M5Tk
//https://www.youtube.com/watch?v=VjuyfBaryu8&t=3s

//������� � ��������� ������
//
//��� � ������������� ���� ��� - TempCubeVertices
//
//������
//���� ����� - ��� ��������� ������� ������� �� ������� ����
//����� ������ ����.
//������� �����. ��� ����������� �������
//���� �� �������
//�������� ��� � Legends of Amberland The Forgotten Crown
//
//��� ������� �� �����
//��� ���� � ���� ���� ������ ����� �������� ����� ������ - https://mudgate.itch.io/mudgate
//������������ ����� - ��� �������� ��������� ������ ������, ���� ��� �������� - ���������� (� ����� ������) ��� ����������
//����� ��������� ����� � ������. ������� �������� �� �����
//
//�������� ��� ������ ����� ������ - https://forum.zdoom.org/viewtopic.php?t=63994

bool GameAppInit()
{
	if( !Tile3DManager::Create() )
		return false;

	PlayerCamera::SetPosition({ 0.0f, 0.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	//SetMouseVisible(false);

	return true;
}

void GameAppClose()
{
	Tile3DManager::Destroy();
}

void GameAppFrame()
{
	PlayerCamera::Update(true, false);

	Matrix4 view = PlayerCamera::GetView();
	Matrix4 perpective = Matrix4::Perspective(45.0f, GetWindowAspectRatio(), 0.01f, 1000.f);
	Tile3DManager::BeginDraw(perpective, view);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // TODO: ���������
	for( size_t x = 0; x < 50; x++ )
	{
		for( size_t y = 0; y < 50; y++ )
		{
			for( size_t z = 0; z < 5; z++ )
			{
				Vector3 pos;
				pos.x = x*2;
				pos.y = z*2;
				pos.z = y*2;
				Tile3DManager::DrawWall(pos);
			}
		}
	}
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);

	//DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, { -10.0f, 2.0f, 0.0f }, RED);
	//DebugDraw::Flush(perpective * view);

	DebugText::Begin();
	DebugText::SetForeground({ 255, 255, 0, 255 });
	DebugText::SetBackground({ 100, 120, 255, 255 });
	DebugText::Print(1, 1, "Hello");
	DebugText::Flush();
}