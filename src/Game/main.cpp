#include "AppConfig.h"
#include "MicroEngine.h"
#if START_EXAMPLE
#include "ExampleDemo.h"
#endif

//повторить вот это 
//https://gpvoid.itch.io/rogue3d - механика бо€
//https://orbissan.itch.io/pixel-roguelike - продолжение дл€ предыдущего - стиль
//https://gpvoid.itch.io/dungeon-crawler - геймплей
//https://wirelessjava.itch.io/amulet-of-destiny
//вот такой еще стиль рендера делать 
//		- https://www.youtube.com/watch?v=JgfACGvWiM4
//		- https://www.youtube.com/watch?v=Eb9C6_43K1Y
//а вообще мир состоит из клеток-тайлов чтобы получить примерно такое - https://mudgate.itch.io/mudgate
//	¬ кубизме кубы равны 1х1х1. у мен€ же один будет 1х”х1 - то есть высота (а в будущем и другие стороны) вариативна

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