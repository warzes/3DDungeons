#include <string>
#include "UnitTestMath.h"

void consoleOkLog(const std::string& msg)
{
	std::string message = "[ \033[32mINFO\033[0m   ] : " + msg;
	puts(message.c_str());
}

void consoleErrorLog(const std::string& msg)
{
	std::string message ="[ \033[31mERROR\033[0m   ] : " + msg;
	puts(message.c_str());
}

void RunTest()
{
	consoleOkLog("UNIT TEST Enable");
	RunUnitTestMath();
}
