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

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER

#include "MicroRender.h"
#include "MicroMath.h"
#include "MicroCollisions.h"
#include "MicroGraphics.h"
#include "MicroAdvance.h"

//=============================================================================
// Core Func
//=============================================================================
template<class T>
inline void Swap(T& left, T& right) noexcept
{
	T tmp = std::move(left);
	left = std::move(right);
	right = std::move(tmp);
}

// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
template<typename T, size_t N>
[[nodiscard]] constexpr size_t Countof(T const (&)[N])
{
	return N;
}

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

enum class MouseButton
{
	Left,
	Middle,
	Right
};

[[nodiscard]] bool IsKeyDown(uint8_t key);
[[nodiscard]] bool IsKeyPressed(uint8_t key);

[[nodiscard]] bool IsMouseButtonDown(MouseButton button);
[[nodiscard]] bool IsMouseButtonClick(MouseButton button);

void SetCursorPosition(int x, int y);     // Set mouse position XY
[[nodiscard]] Point2 GetCursorPosition(); // Get mouse position XY
[[nodiscard]] Point2 GetCursorDelta();    // Get mouse delta between frames
void SetMouseVisible(bool visible);       // Enables/Disables cursor(lock/unlock cursor)

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