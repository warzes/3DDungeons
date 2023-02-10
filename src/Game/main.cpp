#include "AppConfig.h"
#include "MicroEngine.h"
#if START_EXAMPLE
#include "ExampleDemo.h"
#endif

//��������� ��� ��� 
//https://gpvoid.itch.io/rogue3d - �������� ���
//https://orbissan.itch.io/pixel-roguelike - ����������� ��� ����������� - �����
//https://gpvoid.itch.io/dungeon-crawler - ��������
//https://wirelessjava.itch.io/amulet-of-destiny
//��� ����� ��� ����� ������� ������ 
//		- https://www.youtube.com/watch?v=JgfACGvWiM4
//		- https://www.youtube.com/watch?v=Eb9C6_43K1Y
//� ������ ��� ������� �� ������-������ ����� �������� �������� ����� - https://mudgate.itch.io/mudgate
//	� ������� ���� ����� 1�1�1. � ���� �� ���� ����� 1���1 - �� ���� ������ (� � ������� � ������ �������) ����������

//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
#if START_UNIT_TEST
	extern void RunTest();
	RunTest();
#endif // START_UNIT_TEST

#if START_EXAMPLE
	AppSystemCreateInfo createInfo;
	createInfo.window.Vsync = true;
	createInfo.window.Width = 1600;
	createInfo.window.Height = 900;
	if (AppSystemCreate(createInfo))
	{
		ExampleInit();
		while (!IsAppExitRequested())
		{
			AppSystemBeginFrame();

			if (IsKeyDown(27/*VK_ESCAPE*/))
				AppExitRequest();

			ExampleFrame();

			AppSystemEndFrame();
		}
		ExampleClose();
	}
	AppSystemDestroy();
#else
	AppSystemCreateInfo createInfo;
	if (AppSystemCreate(createInfo))
	{
		while (!IsAppExitRequested())
		{
			AppSystemBeginFrame();

			if (IsKeyDown(27/*VK_ESCAPE*/))
				AppExitRequest();

			AppSystemEndFrame();
		}
	}
	AppSystemDestroy();
#endif
}
//-----------------------------------------------------------------------------