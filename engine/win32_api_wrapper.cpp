/*
	Win32 API Wrapper
	This file is part of the K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifdef _WIN32

#include <os_api_wrapper.hpp>

#include <cassert>
#include <iostream>

#include <tchar.h>
#include <windowsx.h>

#define WINDOWCLASSNAME(name) LPCSTR(name)
#define K_ENGINE_WINDOW_CLASS "KENGINE_WINDOW_CLASS"

static kengine::events_callback* globalUserEventsCallback = nullptr;
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
	Registers a Win32 window class
*/
int registerWindowClass(std::string windowClassName);

/*
	Unregistering Win32 window class
*/
int unregisterWindowClass(std::string windowClassName);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	return main();
}

int64_t kengine::getHighResolutionTimerCounter()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return static_cast<int64_t>(time.QuadPart);
}

int64_t kengine::getHighResolutionTimerFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return static_cast<int64_t>(frequency.QuadPart);
}

/*
	Creating a debug console
*/
int kengine::createDebugConsole()
{
	if (!AllocConsole())
	{
		return 0;
	}

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();
	HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();

	return 1;
}

/*
	Closing the debug console
*/
int kengine::closeDebugConsole()
{
	return FreeConsole();
}

/*
	OS initialization
	This function must be called only one time
*/
int kengine::osInitialize()
{
	return registerWindowClass(K_ENGINE_WINDOW_CLASS);
}

/*
	OS finish
*/
int kengine::osFinish()
{
	return unregisterWindowClass(K_ENGINE_WINDOW_CLASS);;
}

/*
	Get the primary display size in pixels
*/
void kengine::getDisplaySize(int* width, int* height)
{
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CYSCREEN);
}

/*
	Get the center position (x axis) of the primary display
*/
int kengine::getDisplayCenterPosX(int displayWidth)
{
	return  (GetSystemMetrics(SM_CXSCREEN) - displayWidth) / 2;
}

/*
	Get the center position in y axis of the primary display
*/
int kengine::getDisplayCenterPosY(int displayHeight)
{
	return (GetSystemMetrics(SM_CYSCREEN) - displayHeight) / 2;
}

/*
	Get the number of monitors
*/
int kengine::getNumberOfMonitors()
{
	return GetSystemMetrics(SM_CMONITORS);
}

/*
	Set global callback events
*/
void kengine::setGlobalUserEventsCallback(events_callback* evt)
{
	assert(globalUserEventsCallback == nullptr); // trying to override a globalUserEventsCallback
	globalUserEventsCallback = evt;
}

/*
	Message events handling (message pump)
*/
void kengine::handleSystemMessages()
{
	// (!) PLEASE DON'T INCLUDE I/O's STUFF HERE!
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			globalUserEventsCallback->afterFinishEvent();
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/*
	kengine::os_window class member definition
*/

bool kengine::os_window::create(int x, int y, int width, int height, std::string name, WINDOW_STYLE style, unsigned long additional)
{
	DWORD dwExStyle = additional;
	DWORD dwStyle = 0;

	switch (style)
	{
	case WINDOW_STYLE::SPLASH:
		dwStyle = WS_POPUPWINDOW;
		break;

	case WINDOW_STYLE::MINIMAL:
		dwStyle = WS_OVERLAPPED | WS_SYSMENU;
		break;

	case WINDOW_STYLE::MINIMAL_NO_SYS:
		dwStyle = WS_OVERLAPPED;
		break;

	case WINDOW_STYLE::WINDOWED_FULLSCREEN:
		dwStyle = WS_POPUPWINDOW | WS_MAXIMIZE;
		break;

	case WINDOW_STYLE::DEFAULT:
		dwStyle = WS_OVERLAPPEDWINDOW;
		break;

	default:
		dwStyle = WS_OVERLAPPEDWINDOW;
		break;
	}

	RECT windowSize = { 0 };
	windowSize.left = (LONG)0;
	windowSize.right = (LONG)width;
	windowSize.top = (LONG)0;
	windowSize.bottom = (LONG)height;

	AdjustWindowRectEx(&windowSize, dwStyle, FALSE, dwExStyle);

	hWindow = CreateWindowEx(
		dwExStyle,
		WINDOWCLASSNAME("KENGINE_WINDOW_CLASS"),
		LPCSTR(name.c_str()),
		dwStyle,
		x,
		y,
		windowSize.right - windowSize.left,
		windowSize.bottom - windowSize.top,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	if (hWindow == nullptr)
	{
		DWORD error = GetLastError();
		globalUserEventsCallback->debugMessage("It was not possible to create the window: " + std::to_string(error) + "\n");
		return false;
	}

	hDC = GetDC(hWindow);

	if (hDC == nullptr)
	{
		DWORD error = GetLastError();
		globalUserEventsCallback->debugMessage("It was not possible to get the device context: " + std::to_string(error) + "\n");
		return 0;
	}
	
	return true;
}

int kengine::os_window::destroy()
{
	int isSuccessful = 1;

	//if (hRC != nullptr)
	//{
	//	if (!wglMakeCurrent(hDC, nullptr))
	//	{
	//		DWORD error = GetLastError();
	//		K_UNREFERENCED_PARAMETER(error);
	//		K_DEBUG_OUTPUT_RAW(K_DEBUG_ERROR, "It was not possible to release the rendering context: " << error << "\n");
	//	}

	//	if (!wglDeleteContext(hRC))
	//	{
	//		DWORD error = GetLastError();
	//		K_UNREFERENCED_PARAMETER(error);
	//		K_DEBUG_OUTPUT_RAW(K_DEBUG_ERROR, "It was not possible to delete the rendering context: " << error << "\n");
	//	}

	//	hRC = nullptr;
	//}

	if (hDC != nullptr)
	{
		if (!ReleaseDC(hWindow, hDC))
		{
			DWORD error = GetLastError();
			globalUserEventsCallback->debugMessage("It was not possible to release the device context: " + std::to_string(error) + "\n");
		}

		hDC = nullptr;
	}

	if (hWindow != nullptr)
	{
		if (!DestroyWindow(hWindow))
		{
			DWORD error = GetLastError();
			globalUserEventsCallback->debugMessage("It was not possible to destroy a window application: " + std::to_string(error) + "\n");
			isSuccessful = 0;
		}

		hWindow = nullptr;
	}

	return isSuccessful;
}

int kengine::os_window::show(int showType) const
{
	return ShowWindow(hWindow, showType);
}

void kengine::os_window::setWindowText(std::string text)
{
	SetWindowTextA(hWindow, text.c_str());
}

int kengine::os_window::swapBuffers() const
{
	return 1;
}

/*
	Registering Win32 window class
*/
int registerWindowClass(std::string windowClassName)
{
	WNDCLASSEX windowClass{ 0 };

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProcedure; // mandatory
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL); // mandatory
	// windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KENGINE_ICON));
	// windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI__KENGINE_ICON_SM)); 
	windowClass.hIcon = 0;
	windowClass.hIconSm = 0;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = WINDOWCLASSNAME(windowClassName.c_str()); // mandatory

	if (RegisterClassEx(&windowClass) == 0)
	{
		assert(!(globalUserEventsCallback == nullptr));
		DWORD error = GetLastError();
		globalUserEventsCallback->debugMessage("It was not possible to register a " + windowClassName + " class window: " + std::to_string(error));
		return 0;
	}

	return 1;
}

/*
	Unregistering Win32 window class
*/
int unregisterWindowClass(std::string windowClassName)
{
	int ret = UnregisterClass(WINDOWCLASSNAME(windowClassName.c_str()), GetModuleHandle(nullptr));

	if (!ret)
	{
		assert(!(globalUserEventsCallback == nullptr));
		DWORD error = GetLastError();
		globalUserEventsCallback->debugMessage("It was not possible to unregister a " + windowClassName + " class window: " + std::to_string(error));
	}

	return ret;
}

/*
	Win32 window procedure
*/
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(!(globalUserEventsCallback == nullptr));

	switch (uMsg)
	{
	case WM_DESTROY: // 0x0002
		PostQuitMessage(0);
		return 0;

	case WM_MOVE: // 0x0003
		globalUserEventsCallback->onMoveWindowEvent(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_SIZE: // 0x0005
		globalUserEventsCallback->onResizeWindowEvent(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_ACTIVATE: // 0x0006
		if (wParam == WA_ACTIVE)
			globalUserEventsCallback->onResumeEvent();
		else if (wParam == WA_CLICKACTIVE)
			globalUserEventsCallback->onResumeEvent();
		else if (wParam == WA_INACTIVE)
			globalUserEventsCallback->onPauseEvent();
		return 0;

	case WM_CLOSE: // 0X0010
		globalUserEventsCallback->beforeFinishEvent();
		return 0;

	case WM_KEYDOWN: // 0x0100
		globalUserEventsCallback->onKeyboardEvent(wParam, 1);
		return 0;

	case WM_KEYUP: // 0x0101
		globalUserEventsCallback->onKeyboardEvent(wParam, 0);
		return 0;

	case WM_SYSKEYDOWN: //0x0104
		globalUserEventsCallback->onKeyboardSpecialEvent(wParam, 1);
		return 0;

	case WM_SYSKEYUP:
		globalUserEventsCallback->onKeyboardSpecialEvent(wParam, 0);
		return 0;

	case WM_MOUSEMOVE: // 0x0200
		globalUserEventsCallback->onMouseMotionEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONDOWN: // 0x201
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::LEFT, kengine::MOUSE_ACTION::DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONUP: // 0x0202
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::LEFT, kengine::MOUSE_ACTION::UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_RBUTTONDOWN: // 0x0204
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::RIGHT, kengine::MOUSE_ACTION::DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_RBUTTONUP: // 0x0205
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::RIGHT, kengine::MOUSE_ACTION::UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_MBUTTONDOWN: // 0x0207
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::MIDDLE, kengine::MOUSE_ACTION::DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_MBUTTONUP: // 0x0208
		globalUserEventsCallback->onMouseButtonEvent(kengine::MOUSE_BUTTON::MIDDLE, kengine::MOUSE_ACTION::UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

#endif