//=============================================================================
// Header
//=============================================================================
//-----------------------------------------------------------------------------
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

#if defined(__ANDROID__)
#endif // __ANDROID__

#if defined(__linux__)
#endif // __linux__

#if defined(__EMSCRIPTEN__)
#endif // __EMSCRIPTEN__

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER
//-----------------------------------------------------------------------------
//=============================================================================
// Global var
//=============================================================================
//-----------------------------------------------------------------------------
namespace core
{
	FILE* LogFile = nullptr;
#if defined(_WIN32)
	LARGE_INTEGER Frequency = {};
	LARGE_INTEGER CurrentTime = {};
	LARGE_INTEGER PrevTime = {};
#endif // _WIN32
	float DeltaTime = 0.0f;
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
	bool Fullscreen = false;
	bool Minimized = false;
}
//-----------------------------------------------------------------------------
namespace input
{
	uint8_t KeyState[256];
	uint8_t MouseButtonState[3];
	Point2 CursorPosition;
	Point2 CursorMove;
	bool MouseVisible = true;
	bool MouseVisibleInternal = true;

	constexpr uint8_t Up = 0;
	constexpr uint8_t Down = 1;
	constexpr uint8_t Pressed = 2;

	inline size_t getIdMouseButton(MouseButton btn)
	{
		switch( btn )
		{
		case MouseButton::Left:   return 0;
		case MouseButton::Middle: return 2;
		case MouseButton::Right:  return 1;
		}
	}

	// Update mouse clipping region.
	inline void updateMouseClipping()
	{
#if defined(_WIN32)
		if( input::MouseVisibleInternal )
			ClipCursor(nullptr);
		else
		{
			POINT point = { 0 };
			ClientToScreen(window::Win32WindowHandle, &point);
			RECT mouseRect = {
				point.x, 
				point.y,
				point.x + GetWindowWidth(),
				point.y + GetWindowHeight()
			};
			ClipCursor(&mouseRect);
		}
#endif // _WIN32
	}

	// Update mouse visibility and clipping region to the OS.
	inline void updateMouseVisible()
	{
		// When the window is unfocused, mouse should never be hidden
		bool newMouseVisible = window::WindowActive ? input::MouseVisible : true;
		if( newMouseVisible != input::MouseVisibleInternal )
		{
#if defined(_WIN32)
			ShowCursor(newMouseVisible ? TRUE : FALSE);
#endif // _WIN32
			input::MouseVisibleInternal = newMouseVisible;
		}

		updateMouseClipping();
	}

	// Refresh the internally tracked mouse cursor position.
	inline void updateMousePosition()
	{
#if defined(_WIN32)
		POINT screenPosition;
		GetCursorPos(&screenPosition);
		ScreenToClient(window::Win32WindowHandle, &screenPosition);
		CursorPosition.x = screenPosition.x;
		CursorPosition.y = screenPosition.y;
#endif // _WIN32
	}
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
		fputs((msg + "\n").c_str(), core::LogFile);
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
bool IsMouseButtonDown(MouseButton button)
{
	return (input::MouseButtonState[input::getIdMouseButton(button)] != input::Up);
}
//-----------------------------------------------------------------------------
bool IsMouseButtonClick(MouseButton button)
{
	const bool pressed = (input::MouseButtonState[input::getIdMouseButton(button)] == input::Pressed);
	input::MouseButtonState[input::getIdMouseButton(button)] = input::Down;
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
	input::CursorPosition = { x, y };
#if defined(_WIN32)
	POINT pos = { x, y };
	ClientToScreen(window::Win32WindowHandle, &pos);
	SetCursorPos(pos.x, pos.y);
#endif // _WIN32

}
//-----------------------------------------------------------------------------
Point2 GetCursorDelta()
{
	return input::CursorMove;
}
//-----------------------------------------------------------------------------
void SetMouseVisible(bool visible)
{
	if( visible != input::MouseVisible )
	{
		input::MouseVisible = visible;
		input::updateMouseVisible();
	}
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
		{
			bool newFocus = LOWORD(wParam) != WA_INACTIVE;
			if( newFocus != window::WindowActive )
			{
				window::WindowActive = newFocus;
				if( window::WindowActive ) // gainFocusEvent
				{
					if ( window::Minimized) 
						ShowWindow(window::Win32WindowHandle, SW_RESTORE);

					// If fullscreen, automatically restore mouse focus
					if ( window::Fullscreen ) 
						input::updateMouseVisible();
				}
				else // loseFocusEvent
				{
					// input reset
					memset(input::KeyState, 0, sizeof(input::KeyState[0])*Countof(input::KeyState));
					memset(input::MouseButtonState, 0, sizeof(input::MouseButtonState[0]) * Countof(input::MouseButtonState));
					input::CursorMove = { 0 };

					// If fullscreen, minimize on focus loss
					if( window::Fullscreen )
						ShowWindow(window::Win32WindowHandle, SW_MINIMIZE);
					
					// Stop mouse cursor hiding & clipping
					input::updateMouseVisible();
				}
			}
		}
		break;

	case WM_SIZE:
		{
			window::Minimized = (wParam == SIZE_MINIMIZED);
			if( !window::Minimized )
			{
				RECT rect;
				GetClientRect(window::Win32WindowHandle, &rect);
				window::WindowClientWidth = rect.right;
				window::WindowClientHeight = rect.bottom;
				window::WindowClientAspectRatio = static_cast<float>(window::WindowClientWidth) / static_cast<float>(window::WindowClientHeight);
			}

			// If mouse is currently hidden, update the clip region
			if( !input::MouseVisibleInternal )
				input::updateMouseClipping();
		}
		
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
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if( message == WM_LBUTTONDOWN ) input::MouseButtonState[0] = input::Pressed;
		if( message == WM_RBUTTONDOWN ) input::MouseButtonState[1] = input::Pressed;
		if( message == WM_MBUTTONDOWN ) input::MouseButtonState[2] = input::Pressed;
		// Make sure we track the button release even if mouse moves outside the window
		SetCapture(window::Win32WindowHandle);
		// Re-establish mouse cursor hiding & clipping
		if( !input::MouseVisible && input::MouseVisibleInternal )
			input::updateMouseVisible();
		return 0;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		// End capture when there are no more mouse buttons held down
		if ( input::MouseButtonState[0] + input::MouseButtonState[1] + input::MouseButtonState[2] > 0)
			ReleaseCapture();
		if (message == WM_LBUTTONUP) input::MouseButtonState[0] = input::Up;
		if (message == WM_RBUTTONUP) input::MouseButtonState[1] = input::Up;
		if (message == WM_MBUTTONUP) input::MouseButtonState[2] = input::Up;
		return 0;

	case WM_MOUSEMOVE:
		{
			Point2 newPosition = { (int)LOWORD(lParam), (int)HIWORD(lParam) };

			// Do not transmit mouse move when mouse should be hidden, but is not due to no input focus
			if( input::MouseVisibleInternal == input::MouseVisible )
			{
				Point2 delta = newPosition - input::CursorPosition;
				input::CursorMove += delta;
				// Recenter in hidden mouse cursor mode to allow endless relative motion
				if( !input::MouseVisibleInternal && (delta.x != 0 || delta.y != 0) )
					SetCursorPosition(GetWindowWidth()/2, GetWindowHeight()/2);
				else
					input::CursorPosition = newPosition;
			}
			else
				input::CursorPosition = newPosition;
		}
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
#if defined(_WIN32)
// TODO: ?????????????????? ?????? ???????????? ???????????????? (?? ?????? ?????????? ?????????????? ?? ????????????????????) ???????????????????? ?????? - https://github.com/alaingalvan/CrossWindow-Graphics
// Create OpenGL Context
bool CreateWindowContext(bool vsync)
{
	window::Win32DCHandle = GetDC(window::Win32WindowHandle);
	if( !window::Win32DCHandle )
	{
		Fatal("Failed to get DC for window.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR) };
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormat = ChoosePixelFormat(window::Win32DCHandle, &pfd);
	if( !pixelFormat )
	{
		Fatal("ChoosePixelFormat() failed: Cannot find a suitable pixel format.");
		return false;
	}
	DescribePixelFormat(window::Win32DCHandle, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if( !SetPixelFormat(window::Win32DCHandle, pixelFormat, &pfd) )
	{
		Fatal("SetPixelFormat() failed: Cannot set format specified.");
		return false;
	}

	// ?????????????????? ???????? ???? ?????????????????? ?????? ?????? ?????? ?????????????????? ???????????????????????????? (?? ???????????? ?????????? ?????? ???????????? - https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL) - Create a False Context
	HGLRC tempContext = wglCreateContext(window::Win32DCHandle);
	if( !tempContext || !wglMakeCurrent(window::Win32DCHandle, tempContext) )
	{
		Fatal("??reating temp render context fail");
		return false;
	}

	// Get OpenGL Func
	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
	auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)OpenGLGetProcAddress("wglCreateContextAttribsARB");
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
	auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)OpenGLGetProcAddress("wglSwapIntervalEXT");

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(tempContext);

	if( !wglCreateContextAttribsARB )
	{
		Fatal("wglCreateContextAttribsARB fail");
		return false;
	}

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
	const int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	window::Win32ContextHandle = wglCreateContextAttribsARB(window::Win32DCHandle, 0, contextAttribs);
	if( !window::Win32ContextHandle || !wglMakeCurrent(window::Win32DCHandle, window::Win32ContextHandle) )
	{
		Fatal("Creating render context fail");
		return false;
	}

	// Vsync
	if( wglSwapIntervalEXT ) wglSwapIntervalEXT(vsync ? 1 : 0);

	OpenGLInit(OpenGLGetProcAddress);
}
#endif // _WIN32
//-----------------------------------------------------------------------------
bool WindowSystemCreate(const WindowSystemCreateInfo& createInfo)
{
#if defined(_WIN32)

	window::Win32HInstance = GetModuleHandle(nullptr);

	const wchar_t* windowClassName = L"WindowWin32App";

	WNDCLASSEX wndClass    = { sizeof(WNDCLASSEX) };
	wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc   = WindowProc;
	wndClass.hInstance     = window::Win32HInstance;
	wndClass.lpszClassName = windowClassName;
	if (!RegisterClassEx(&wndClass))
	{
		Fatal("Failed To Register The Window Class.");
		return false;
	}

	// Create Window

	const int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - createInfo.Width) / 2;
	const int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - createInfo.Height) / 2;

	const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	const DWORD dwExStyle = 0/*WS_EX_APPWINDOW | WS_EX_WINDOWEDGE*/; // TODO: ????????????-???? ?????????????????? ???????????? ???????????? ???? 20 ?????? ???????????? ???????? (920 ???????????? 20)

	RECT windowRect = { windowPosX, windowPosY, windowPosX + createInfo.Width, windowPosY + createInfo.Height };
	AdjustWindowRect(&windowRect, dwStyle, dwExStyle);

	window::Win32WindowHandle = CreateWindowEx(0, windowClassName, createInfo.Title, dwStyle, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, window::Win32HInstance, nullptr);
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

	if( !CreateWindowContext(createInfo.Vsync) )
		return false;

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
//-----------------------------------------------------------------------------
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
// App System
//=============================================================================
//-----------------------------------------------------------------------------
bool AppSystemCreate(const AppSystemCreateInfo& createInfo)
{
#if defined(_WIN32)
	if (!QueryPerformanceFrequency(&core::Frequency)) return false;
	if (!QueryPerformanceCounter(&core::CurrentTime)) return false;
	core::PrevTime = core::CurrentTime;
#endif

	LogCreate("../log.txt");

	if (!WindowSystemCreate(createInfo.window))
		return false;

	input::updateMouseVisible();
	input::updateMousePosition();

	RenderSystemInit();

	if (!DebugDraw::Init())
		return false;

	if( !DebugText::Init() )
		return false;

	return !IsAppExitRequested();
}
//-----------------------------------------------------------------------------
void AppSystemDestroy()
{
	DebugText::Close();
	DebugDraw::Close();
	ResourceCacheSystem::Clear();
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
	RenderSystemBeginFrame(window::WindowClientWidth, window::WindowClientHeight);
}
//-----------------------------------------------------------------------------
void AppSystemEndFrame()
{
	input::CursorMove = { 0 };
	WindowSystemUpdate();
#if defined(_WIN32)
	QueryPerformanceCounter(&core::CurrentTime);
	double delta = (double)(core::CurrentTime.QuadPart - core::PrevTime.QuadPart);
	delta /= core::Frequency.QuadPart;
	core::DeltaTime = (float)delta;
	core::PrevTime = core::CurrentTime;
#endif
}
//-----------------------------------------------------------------------------
void AppExitRequest()
{
	app::IsExitRequested = true;
}
//-----------------------------------------------------------------------------
float GetDeltaTime()
{
	return core::DeltaTime;
}
//-----------------------------------------------------------------------------