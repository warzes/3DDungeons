#include "AppConfig.h"
#include "MicroEngine.h"
#if START_EXAMPLE
#include "ExampleDemo.h"
#endif
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