#pragma once

#include <string>
void consoleOkLog(const std::string& msg);
void consoleErrorLog(const std::string& msg);

#include "MicroMath.h"

void RunUnitTestMath()
{
	consoleOkLog("MATH TEST Enable");

	//-------------------------------------------------------------------------
	// Core func
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Point2
	//-------------------------------------------------------------------------
	Point2 p1, p2, p3;
	p3 = p1 + p2;

}