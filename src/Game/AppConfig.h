#pragma once

#define START_UNIT_TEST 1

//=============================================================================
#define START_EXAMPLE 1

#define EXAMPLE_01_TRIANGLE 0
#define EXAMPLE_02_QUADTTEXTURE 0
#define EXAMPLE_03_OBJMODEL 0

#define EXAMPLE_X_TEMP 0
#define EXAMPLE_X_DEBUG 0
#define EXAMPLE_X_TESTFPS 1

#if EXAMPLE_01_TRIANGLE
#	include "01_Triangle.h"
#endif

#if EXAMPLE_02_QUADTTEXTURE
#	include "02_QuadTexture.h"
#endif

#if EXAMPLE_03_OBJMODEL
#	include "03_ObjModel.h"
#endif

#if EXAMPLE_X_TEMP
#	include "X_CurrentTest.h"
#endif

#if EXAMPLE_X_DEBUG
#	include "X_Debug.h"
#endif

#if EXAMPLE_X_TESTFPS
#	include "X_TestFPS.h"
#endif

//=============================================================================
#define START_GAME 0

#define GAME_01_DUNGEONCRAWLER 0

#if GAME_01_DUNGEONCRAWLER
#	include "DCGameApp.h"
#endif