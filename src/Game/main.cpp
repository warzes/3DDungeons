#include "MicroEngine.h"
#include "AppConfig.h"

//сделать демки из https ://github.com/floooh/sokol
//https://mrnr15.itch.io/dungeon-dare

#if defined(_MSC_VER)
#	pragma comment( lib, "PhysicEngine.lib" )
#endif

//повторить вот это 
//https://gpvoid.itch.io/rogue3d - механика боя
//https://orbissan.itch.io/pixel-roguelike - продолжение для предыдущего - стиль
//https://gpvoid.itch.io/dungeon-crawler - геймплей
//https://wirelessjava.itch.io/amulet-of-destiny
// https://genaralskar.itch.io/dungeon-crawler - анимация оружия
//вот такой еще стиль рендера делать 
//		- https://www.youtube.com/watch?v=JgfACGvWiM4
//		- https://www.youtube.com/watch?v=Eb9C6_43K1Y
//а вообще мир состоит из клеток-тайлов чтобы получить примерно такое - https://mudgate.itch.io/mudgate
//	В кубизме кубы равны 1х1х1. у меня же один будет 1хУх1 - то есть высота (а в будущем и другие стороны) вариативна

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
#elif START_GAME
	AppSystemCreateInfo createInfo;
	createInfo.window.Vsync = true;
	createInfo.window.Width = 1600;
	createInfo.window.Height = 900;
	if (AppSystemCreate(createInfo) && GameAppInit() )
	{
		while (!IsAppExitRequested())
		{
			AppSystemBeginFrame();

			if (IsKeyDown(27/*VK_ESCAPE*/))
				AppExitRequest();

			GameAppFrame();

			AppSystemEndFrame();
}
		GameAppClose();
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