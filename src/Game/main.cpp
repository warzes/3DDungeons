#include "AppConfig.h"
#include "MicroEngine.h"
#if START_EXAMPLE
#include "ExampleDemo.h"
#endif
#include <time.h>

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
	if (AppSystemCreate(createInfo))
	{
		ExampleInit();
		while (!IsAppExitRequested())
		{
			double d1 = double(clock()) / CLOCKS_PER_SEC;
			AppSystemBeginFrame();

			if (IsKeyDown(27/*VK_ESCAPE*/))
				AppExitRequest();

			ExampleFrame();

			AppSystemEndFrame();
			double d2 = double(clock()) / CLOCKS_PER_SEC;

			double dt = d2 - d1;
			puts(std::to_string(dt).c_str());

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