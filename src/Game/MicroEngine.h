#pragma once

//=============================================================================
// Engine Config
//=============================================================================

//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 5264)
#endif // _MSC_VER

#include <assert.h>
#include <string>
#include <vector>

#include "MicroOpenGLLoader.h"
#include "MicroMath.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER

//=============================================================================
// Logging System
//=============================================================================
void LogCreate(const std::string& fileName);
void LogDestroy();
void LogPrint(const std::string& msg);
void LogWarning(const std::string& msg);
void LogError(const std::string& msg);
void Fatal(const std::string& msg);

//=============================================================================
// Input System
//=============================================================================

[[nodiscard]] bool IsKeyDown(uint8_t key);
[[nodiscard]] bool IsKeyPressed(uint8_t key);

[[nodiscard]] bool IsMouseButtonDown(uint8_t button);
[[nodiscard]] bool IsMouseButtonClick(uint8_t button);
[[nodiscard]] Point2 GetCursorPosition();
void SetCursorPosition(int x, int y);

//=============================================================================
// Window System
//=============================================================================
struct WindowSystemCreateInfo
{
	int Width = 1024;
	int Height = 768;
	const wchar_t* Title = L"Game";
	bool Vsync = true;
};
[[nodiscard]] bool WindowSystemCreate(const WindowSystemCreateInfo& createInfo);
void WindowSystemDestroy();
[[nodiscard]] bool WindowSystemShouldClose();
void WindowSystemUpdate();

[[nodiscard]] int GetWindowWidth();
[[nodiscard]] int GetWindowHeight();
[[nodiscard]] float GetWindowAspectRatio();

//=============================================================================
// Render System
//=============================================================================

void RenderSystemInit();
void RenderSystemBeginFrame();

//=============================================================================
// App System
//=============================================================================
struct AppSystemCreateInfo
{
	WindowSystemCreateInfo window;
};

[[nodiscard]] bool AppSystemCreate(const AppSystemCreateInfo& createInfo);
void AppSystemDestroy();

[[nodiscard]] bool IsAppExitRequested();

void AppSystemBeginFrame();
void AppSystemEndFrame();

void AppExitRequest();
[[nodiscard]] float GetDeltaTime();

