#include "AppConfig.h"
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
#if START_UNIT_TEST
	extern void RunTest();
	RunTest();
#endif // START_UNIT_TEST
}