//=============================================================================
// Header
//=============================================================================

#if defined(_MSC_VER)
#	pragma warning(push)
//#	pragma warning(disable : 4005)
#	pragma warning(disable : 4365)
#	pragma warning(disable : 4820)
#	pragma warning(disable : 5039)
#	pragma warning(disable : 5045)
#	pragma warning(disable : 5219)
#	pragma warning(disable : 5262)
//#	pragma warning(disable : 5264)
#endif // _MSC_VER

#include "MicroEngine.h"

#if defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN
#	define WIN_32_EXTRA_LEAN
#	define _WIN32_WINNT 0x0600
#	define WINVER 0x0600
#	define NOMINMAX
#	include <SDKDDKVer.h>
#	include <winapifamily.h>
#	include <windows.h>
#endif // _WIN32

#if defined(__linux__)
#endif // __linux__

#if defined(__EMSCRIPTEN__)
#endif // __EMSCRIPTEN__

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER

//=============================================================================
// Global var
//=============================================================================
//-----------------------------------------------------------------------------
namespace core
{
	FILE* LogFile = nullptr;
}
//-----------------------------------------------------------------------------
namespace window
{
#if defined(_WIN32)
	HINSTANCE Win32HInstance = nullptr;
	HWND Win32WindowHandle = nullptr;
	HDC Win32DCHandle = nullptr;
	HGLRC Win32ContextHandle = nullptr;
	MSG Win32Msg = {};
#endif // _WIN32
	int WindowClientWidth = 0;
	int WindowClientHeight = 0;
	float WindowClientAspectRatio = 0.0f;
	bool IsWindowRunning = false;
	bool WindowActive = true;
}
//-----------------------------------------------------------------------------
namespace input
{
	uint8_t KeyState[256];
	uint8_t MouseButtonState[3];
	Point2 CursorPosition;

	constexpr uint8_t Up = 0;
	constexpr uint8_t Down = 1;
	constexpr uint8_t Pressed = 2;
}
//-----------------------------------------------------------------------------
namespace render
{
	int FramebufferWidth = 0;
	int FramebufferHeight = 0;

	Color ClearColor;
	float PerspectiveFOV = 0.0f;
	float PerspectiveNear = 0.01f;
	float PerspectiveFar = 1000.0f;
	Matrix4 ProjectionMatrix;
	Matrix4 OrthoMatrix;
}
//-----------------------------------------------------------------------------
namespace app
{
	bool IsExitRequested = false;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Logging
//=============================================================================
//-----------------------------------------------------------------------------
void LogCreate(const std::string& fileName)
{
	errno_t fileErr = fopen_s(&core::LogFile, fileName.c_str(), "w");
	if (fileErr != 0 || !core::LogFile)
	{
		LogError("LogCreate() failed!!!");
		core::LogFile = nullptr;
	}
}
//-----------------------------------------------------------------------------
void LogDestroy()
{
	if (core::LogFile)
	{
		fclose(core::LogFile);
		core::LogFile = nullptr;
	}
}
//-----------------------------------------------------------------------------
void LogPrint(const std::string& msg)
{
	puts(msg.c_str());
	if (core::LogFile)
	{
		fputs(msg.c_str(), core::LogFile);
		fputs("\n", core::LogFile);
	}
}
//-----------------------------------------------------------------------------
void LogWarning(const std::string& msg)
{
	LogPrint("Warning: " + msg);
}
//-----------------------------------------------------------------------------
void LogError(const std::string& msg)
{
	LogPrint("Error: " + msg);
}
//-----------------------------------------------------------------------------
void Fatal(const std::string& msg)
{
	app::IsExitRequested = true;
	LogError(msg);
}
//-----------------------------------------------------------------------------
//=============================================================================
// Input System
//=============================================================================
//-----------------------------------------------------------------------------
bool IsKeyDown(uint8_t key)
{
	return (input::KeyState[key] != input::Up);
}
//-----------------------------------------------------------------------------
bool IsKeyPressed(uint8_t key)
{
	const bool pressed = (input::KeyState[key] == input::Pressed);
	input::KeyState[key] = input::Down;
	return pressed;
}
//-----------------------------------------------------------------------------
bool IsMouseButtonDown(uint8_t button)
{
	return (input::MouseButtonState[button] != input::Up);
}
//-----------------------------------------------------------------------------
bool IsMouseButtonClick(uint8_t button)
{
	const bool pressed = (input::MouseButtonState[button] == input::Pressed);
	input::MouseButtonState[button] = input::Down;
	return pressed;
}
//-----------------------------------------------------------------------------
Point2 GetCursorPosition()
{
	return input::CursorPosition;
}
//-----------------------------------------------------------------------------
void SetCursorPosition(int x, int y)
{
#if defined(_WIN32)
	POINT pos = { x, y };
	ClientToScreen(window::Win32WindowHandle, &pos);
	SetCursorPos(pos.x, pos.y);
#endif // _WIN32
	input::CursorPosition = { x, y };
}
//-----------------------------------------------------------------------------
//=============================================================================
// Window System
//=============================================================================
//-----------------------------------------------------------------------------
void* OpenGLGetProcAddress(const char* funcName)
{
#if defined(_WIN32)
	void* ptrFunc = (void*)wglGetProcAddress(funcName);
	if (!ptrFunc)
		Fatal("Loading extension '" + std::string(funcName) + "' fail (" + std::to_string(GetLastError()) + ")");
	return ptrFunc;
#endif // _WIN32
}
//-----------------------------------------------------------------------------
#if defined(_WIN32)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		window::WindowActive = (message == WM_SETFOCUS);
		return 0;

	case WM_ACTIVATE:
		window::WindowActive = (LOWORD(wParam) != WA_INACTIVE);
		return 0;

	case WM_SIZE:
		window::WindowClientWidth = LOWORD(lParam);
		window::WindowClientHeight = HIWORD(lParam);
		window::WindowClientAspectRatio = static_cast<float>(window::WindowClientWidth) / static_cast<float>(window::WindowClientHeight);
		break;

		//case WM_CHAR:
		//	switch( wParam )
		//	{
		//	case 27: /* ESC key */
		//		PostQuitMessage(0);
		//		break;
		//	}
		//	return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256 && (lParam & 0x40000000) == 0)
			input::KeyState[wParam] = input::Pressed;
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256) input::KeyState[wParam] = input::Up;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		input::CursorPosition.x = (int)LOWORD(lParam);
		input::CursorPosition.y = (int)HIWORD(lParam);

		if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
			input::MouseButtonState[0] = (message == WM_LBUTTONDOWN ? input::Pressed : input::Up);
		if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
			input::MouseButtonState[1] = (message == WM_RBUTTONDOWN ? input::Pressed : input::Up);
		if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
			input::MouseButtonState[2] = (message == WM_MBUTTONDOWN ? input::Pressed : input::Up);

		return 0;

	case WM_MOUSEMOVE:
		input::CursorPosition.x = (int)LOWORD(lParam);
		input::CursorPosition.y = (int)HIWORD(lParam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_ERASEBKGND:
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif // _WIN32
//-----------------------------------------------------------------------------
bool WindowSystemCreate(const WindowSystemCreateInfo& createInfo)
{
#if defined(_WIN32)

	window::Win32HInstance = GetModuleHandle(nullptr);

	const wchar_t* windowClassName = L"WindowWin32App";

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = window::Win32HInstance;
	wcex.lpszClassName = windowClassName;
	if (!RegisterClassEx(&wcex))
	{
		Fatal("Failed To Register The Window Class.");
		return false;
	}

	// Create Window

	const int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - createInfo.Width) / 2;
	const int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - createInfo.Height) / 2;

	const DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT windowRect = { windowPosX, windowPosY, windowPosX + createInfo.Width, windowPosY + createInfo.Height };
	AdjustWindowRect(&windowRect, style, 0);

	window::Win32WindowHandle = CreateWindowEx(0, windowClassName, createInfo.Title, style, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, window::Win32HInstance, nullptr);
	if (!window::Win32WindowHandle)
	{
		Fatal("CreateWindow() failed:  Cannot create a window.");
		return false;
	}
	ShowWindow(window::Win32WindowHandle, SW_SHOW);
	UpdateWindow(window::Win32WindowHandle);

	RECT clientRect;
	GetClientRect(window::Win32WindowHandle, &clientRect);
	window::WindowClientWidth = clientRect.right - clientRect.left;
	window::WindowClientHeight = clientRect.bottom - clientRect.top;

	// Create OpenGL Context

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);

	window::Win32DCHandle = GetDC(window::Win32WindowHandle);
	if (!window::Win32DCHandle)
	{
		Fatal("Failed to get DC for window.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR) };
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(window::Win32DCHandle, &pfd);
	if (!pixelFormat)
	{
		Fatal("ChoosePixelFormat() failed: Cannot find a suitable pixel format.");
		return false;
	}
	DescribePixelFormat(window::Win32DCHandle, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (!SetPixelFormat(window::Win32DCHandle, pixelFormat, &pfd))
	{
		Fatal("SetPixelFormat() failed: Cannot set format specified.");
		return false;
	}

	// временное окно не создается так как нет установки пиксельформата (а обычно стоит его делать - https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL) - Create a False Context
	HGLRC tempContext = wglCreateContext(window::Win32DCHandle);
	if (!tempContext || !wglMakeCurrent(window::Win32DCHandle, tempContext))
	{
		Fatal("Сreating temp render context fail");
		return false;
	}

	// Get OpenGL Func
	auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)OpenGLGetProcAddress("wglCreateContextAttribsARB");
	auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)OpenGLGetProcAddress("wglSwapIntervalEXT");

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(tempContext);

	if (!wglCreateContextAttribsARB)
	{
		Fatal("wglCreateContextAttribsARB fail");
		return false;
	}

	const int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	window::Win32ContextHandle = wglCreateContextAttribsARB(window::Win32DCHandle, 0, contextAttribs);
	if (!window::Win32ContextHandle || !wglMakeCurrent(window::Win32DCHandle, window::Win32ContextHandle))
	{
		Fatal("Creating render context fail");
		return false;
	}

	// Vsync
	if (wglSwapIntervalEXT) wglSwapIntervalEXT(createInfo.Vsync ? 1 : 0);

	OpenGLInit(OpenGLGetProcAddress);

	window::IsWindowRunning = true;
	return true;
#endif // _WIN32
}
//-----------------------------------------------------------------------------
void WindowSystemDestroy()
{
#if defined(_WIN32)
	window::IsWindowRunning = false;
	if (window::Win32ContextHandle)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(window::Win32ContextHandle);
		window::Win32ContextHandle = nullptr;
	}
	if (window::Win32DCHandle)
	{
		ReleaseDC(window::Win32WindowHandle, window::Win32DCHandle);
		window::Win32DCHandle = nullptr;
	}
	if (window::Win32WindowHandle)
	{
		DestroyWindow(window::Win32WindowHandle);
		window::Win32WindowHandle = nullptr;
	}
#endif // _WIN32
}
//-----------------------------------------------------------------------------
bool WindowSystemShouldClose()
{
	return !window::IsWindowRunning;
}

void WindowSystemUpdate()
{
#if defined(_WIN32)
	SwapBuffers(window::Win32DCHandle);

	while (PeekMessage(&window::Win32Msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (window::Win32Msg.message == WM_QUIT)
		{
			window::IsWindowRunning = false;
			break;
		}
		TranslateMessage(&window::Win32Msg);
		DispatchMessage(&window::Win32Msg);
	}
#endif // _WIN32
}
//-----------------------------------------------------------------------------
int GetWindowWidth()
{
	return window::WindowClientWidth;
}
//-----------------------------------------------------------------------------
int GetWindowHeight()
{
	return window::WindowClientHeight;
}
//-----------------------------------------------------------------------------
float GetWindowAspectRatio()
{
	return window::WindowClientAspectRatio;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Render System
//=============================================================================
//-----------------------------------------------------------------------------
void RenderSystemInit()
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthRange(0.0f, 1.0f);
}
//-----------------------------------------------------------------------------
void RenderSystemBeginFrame()
{
	if (render::FramebufferWidth != window::WindowClientWidth || render::FramebufferHeight != window::WindowClientHeight)
	{
		render::FramebufferWidth = window::WindowClientWidth;
		render::FramebufferHeight = window::WindowClientHeight;

		glViewport(0, 0, render::FramebufferWidth, render::FramebufferHeight);
		glScissor(0, 0, render::FramebufferWidth, render::FramebufferHeight);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
//=============================================================================
// App System
//=============================================================================
//-----------------------------------------------------------------------------
bool AppSystemCreate(const AppSystemCreateInfo& createInfo)
{
	LogCreate("../log.txt");

	if (!WindowSystemCreate(createInfo.window))
		return false;

	void RenderSystemInit();

	return !IsAppExitRequested();
}
//-----------------------------------------------------------------------------
void AppSystemDestroy()
{
	WindowSystemDestroy();
	LogDestroy();
}
//-----------------------------------------------------------------------------
bool IsAppExitRequested()
{
	return app::IsExitRequested || WindowSystemShouldClose();
}
//-----------------------------------------------------------------------------
void AppSystemBeginFrame()
{
	RenderSystemBeginFrame();
}
//-----------------------------------------------------------------------------
void AppSystemEndFrame()
{
	WindowSystemUpdate();
}
//-----------------------------------------------------------------------------
void AppExitRequest()
{
	app::IsExitRequested = true;
}
//-----------------------------------------------------------------------------