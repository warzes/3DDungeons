#include "AppConfig.h"
#include "MicroEngine.h"
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
#if START_UNIT_TEST
	extern void RunTest();
	RunTest();
#endif // START_UNIT_TEST

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
}
//-----------------------------------------------------------------------------