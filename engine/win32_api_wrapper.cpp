/*
	Win32 API Wrapper
	This file is part of the K-Engine.

	Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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

#if defined(_WIN32)

// k-engine headers
#include <os_api_wrapper.hpp>
#include <k_version.hpp>
// std headers
#include <cassert>
#include <iostream>
// microsoft windows headers
#include <tchar.h>
#include <windowsx.h>

/*
	Global Objects
*/

static kengine::events_callback* globalUserEventsCallback = nullptr;
static kengine::global_app_manager globalAppManager;

/*
	WIN32 API Global Objects
*/

#define WINDOWCLASSNAME(name) LPCSTR(name)
#define K_ENGINE_WINDOW_CLASS "KENGINE_WINDOW_CLASS"

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
	OpenGL Procedures Extension for Win32 (WGL)
*/

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 0;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;

/*
	Registers a Win32 window class
*/
int registerWindowClass(std::string windowClassName);


/*
	Unregistering a Win32 window class
*/
int unregisterWindowClass(std::string windowClassName);

/*
	C++ program entry point
*/
int main();

/*
	Win32 program entry point
*/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	return main();
}

/*
	K-Engine public interface for OS API functions
*/
namespace kengine
{
	class win32_window : public window
	{
		friend class win32_rendering_context;

	public:
		win32_window() {}
		
		~win32_window() {
			if (hWindow)
				destroy();
		}

		win32_window(const win32_window& copy) = delete; // copy constructor
		win32_window& operator=(const win32_window& copy) = delete; // copy assignment
		win32_window(win32_window&& move) noexcept = delete;  // move constructor
		win32_window& operator=(win32_window&&) = delete; // move assigment

		bool create(int xPos, int yPos, int widthParam, int heightParam, const std::string& nameParam) {
			x = xPos;
			y = yPos;
			width = widthParam;
			height = heightParam;
			name = nameParam;

			//style = styleParam;
			//additionalStyle = static_cast<WINDOW_ADDITIONAL_STYLE>(additional);

			//DWORD dwExStyle = additional;
			DWORD dwExStyle = WS_EX_APPWINDOW;
			DWORD dwStyle = 0;

			//switch (style)
			//{
			//case WINDOW_STYLE::SPLASH:
			//	dwStyle = WS_POPUPWINDOW;
			//	break;

			//case WINDOW_STYLE::MINIMAL:
			//	dwStyle = WS_OVERLAPPED | WS_SYSMENU;
			//	break;

			//case WINDOW_STYLE::MINIMAL_NO_SYS:
			//	dwStyle = WS_OVERLAPPED;
			//	break;

			//case WINDOW_STYLE::WINDOWED_FULLSCREEN:
			//	dwStyle = WS_POPUPWINDOW | WS_MAXIMIZE;
			//	break;

			//case WINDOW_STYLE::DEFAULT:
			dwStyle = WS_OVERLAPPEDWINDOW;
			//	break;

			//default:
			//	dwStyle = WS_OVERLAPPEDWINDOW;
			//	break;
			//}

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

			if (hWindow == nullptr) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to create the window: " + std::to_string(error) + "\n");
				return false;
			}

			hDC = GetDC(hWindow);

			if (hDC == nullptr) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to get the device context: " + std::to_string(error) + "\n");
				return 0;
			}

			if (globalUserEventsCallback && globalAppManager.isCallbackCallsEnabled())
				globalUserEventsCallback->onWindowReady(this);

			return true;
		}

		bool create() {
			return create(x, y, width, height, name);
		}

		int destroy() {
			int isSuccessful = 1;

			if (hDC != nullptr) {
				if (!ReleaseDC(hWindow, hDC)) {
					DWORD error = GetLastError();
					globalUserEventsCallback->debugMessage("It was not possible to release the device context: " + std::to_string(error) + "\n");
				}

				hDC = nullptr;
			}

			if (hWindow != nullptr) {
				if (!DestroyWindow(hWindow)) {
					DWORD error = GetLastError();
					globalUserEventsCallback->debugMessage("It was not possible to destroy a window application: " + std::to_string(error) + "\n");
					isSuccessful = 0;
				}

				hWindow = nullptr;
			}

			return isSuccessful;
		}

		int show() {
			return ShowWindow(hWindow, SW_SHOW);
		}

		int hide() {
			return ShowWindow(hWindow, SW_HIDE);
		}

		int swapBuffers() const {
			return SwapBuffers(hDC);
		}

		void setWindowText(std::string text) {
			SetWindowTextA(hWindow, text.c_str());
		}

		void* getHandle() {
			return hWindow;
		}

	private:
		HWND hWindow = NULL;
		HDC hDC = NULL;
	};

	window* windowInstance()
	{
		return new win32_window;
	}

	/*
	*/
	class win32_rendering_context : public rendering_context
	{
	public:
		explicit win32_rendering_context(window* windowParam) {
			assert(!(windowParam == nullptr));
			win32_win = dynamic_cast<win32_window*>(windowParam);
		}

		~win32_rendering_context() {
			destroy();
		}

		win32_rendering_context(const win32_rendering_context& copy) = delete; // copy constructor
		win32_rendering_context& operator=(const win32_rendering_context& copy) = delete; // copy assignment
		win32_rendering_context(win32_rendering_context&& move) noexcept = delete;  // move constructor
		win32_rendering_context& operator=(win32_rendering_context&&) = delete; // move assigment

		int create(const compatibility_profile& profile) {
			/*
				We need to create a dummy opengl context first to get the WGL extensions to create a better OpenGL context.

				https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
			*/

			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				32, // cColorBits
				0, // cRedBits
				0, // cRedShift
				0, // cGreenBits
				0, // cGreenShift
				0, // cBlueBits
				0, // cBlueShift
				0, // cAlphaBits
				0, // cAlphaShift
				0, // cAccumBits
				0, // cAccumRedBits
				0, // cAccumGreenBits
				0, // cAccumBlueBits
				0, // cAccumAlphaBits
				24, // cDepthBits
				8, // cStencilBits
				0, // cAuxBuffers
				PFD_MAIN_PLANE, // iLayerType
				0, // bReserved
				0, // dwLayerMask
				0, // dwVisibleMask
				0 // dwDamageMask
			};

			int pixelFormat = ChoosePixelFormat(win32_win->hDC, &pfd);

			if (pixelFormat == 0) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to choose an pixel format: " + std::to_string(error) + "\n");
				return 0;
			}

			if (!SetPixelFormat(win32_win->hDC, pixelFormat, &pfd)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to set the format pixel: " + std::to_string(error) + "\n");
				return 0;
			}

			hRC = wglCreateContext(win32_win->hDC);

			if (hRC == nullptr) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to create the rendering context: " + std::to_string(error) + "\n");
				return 0;
			}

			makeCurrent(true);

			if (!kengine::getAllGLProcedureAddress())
				return 0;

			globalAppManager.setCallbackCalls(false);
			// destroying a dummy opengl context
			destroy();
			// destroying the window
			win32_win->destroy();
			// flush the Window event messages
			kengine::handleSystemMessages();
			// creating a new window for the new opengl context (extended context)
			win32_win->create();
			// creating a new rendering context with WGL extension
			createEXT(profile);
			makeCurrent(true);
			globalAppManager.setCallbackCalls(true);
			return 1;
		}

		int destroy() {
			if (!wglMakeCurrent(win32_win->hDC, nullptr)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to make the current rendering context nullptr: " + std::to_string(error) + "\n");
			}

			if (!wglDeleteContext(hRC)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to delete the rendering context: " + std::to_string(error) + "\n");
				return 0;
			}

			hRC = nullptr;
			return 1;
		}

		int makeCurrent(bool enable) {
			HGLRC context = (enable ? hRC : nullptr);

			if (!wglMakeCurrent(win32_win->hDC, context)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to make current the rendering context: " + std::to_string(error) + "\n");
				return 0;
			}

			return 1;
		}

		int createEXT(const compatibility_profile& profile) {
			/*
				check https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt for more details
			*/

#ifndef WGL_ARB_extensions_string
			globalUserEventsCallback->debugMessage("The WGL_ARB_extensions_string extension is not defined");
			return 0;
#endif

#ifndef WGL_ARB_pixel_format
			globalUserEventsCallback->debugMessage("The WGL_ARB_pixel_format extension is not defined");
			return 0;
#endif

#ifndef WGL_ARB_create_context
			globalUserEventsCallback->debugMessage("The WGL_ARB_create_context extension is not defined");
			return 0;
#endif

//#ifndef WGL_ARB_create_context_profile
//			globalUserEventsCallback->debugMessage("The WGL_ARB_create_context_profile extension is not defined");
//			return 0;
//#endif

#ifndef WGL_EXT_swap_control
			globalUserEventsCallback->debugMessage("The WGL_EXT_swap_control extension is not defined");
			return 0;
#endif

//#ifndef GL_ARB_compatibility
//			globalUserEventsCallback->debugMessage("The GL_ARB_compatibility extension is not defined");
//			return 0;
//#endif

			/*
				https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
			*/

			const int piAttribIList[] = {
				WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
				WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,		32,
				WGL_DEPTH_BITS_ARB,		24,
				WGL_STENCIL_BITS_ARB,	8,
				0, // end list
			};

			int piFormats;
			UINT nNumFormats;

			if (wglChoosePixelFormatARB(win32_win->hDC, piAttribIList, nullptr, 1, &piFormats, &nNumFormats) == FALSE) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to choose an extensible pixel format: " + std::to_string(error) + "\n");
				return 0;
			}

			PIXELFORMATDESCRIPTOR pfd;
			if (!DescribePixelFormat(win32_win->hDC, piFormats, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to obtains information about the ext pixel format: " + std::to_string(error) + "\n");
				return 0;
			}

			if (!SetPixelFormat(win32_win->hDC, piFormats, &pfd)) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to set the ext format pixel: " + std::to_string(error) + "\n");
				return 0;
			}

			if (hRC != nullptr)
				destroy();

			const int attribList[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, KENGINE_OPENGL_MAJOR_VERSION,
				WGL_CONTEXT_MINOR_VERSION_ARB, KENGINE_OPENGL_MINOR_VERSION,
				//WGL_CONTEXT_LAYER_PLANE_ARB,
				WGL_CONTEXT_FLAGS_ARB, profile.contextFlag, // WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, WGL_CONTEXT_DEBUG_BIT_ARB
				WGL_CONTEXT_PROFILE_MASK_ARB, profile.profileMask, // WGL_CONTEXT_CORE_PROFILE_BIT_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 
				0
			};

			// wglShareLists 

			hRC = wglCreateContextAttribsARB(win32_win->hDC, hRC, attribList);

			if (hRC == nullptr) {
				DWORD error = GetLastError();
				globalUserEventsCallback->debugMessage("It was not possible to create the ext rendering context: " + std::to_string(error) + "\n");
				return 0;
			}

			return 1;
		}

		int swapBuffers() {
			return 1;
		}

		void clearBuffers() {
			glClear(GL_COLOR_BUFFER_BIT);
		}

	private:
		win32_window* win32_win = nullptr; // avoid dynamic casts in the methods
		HGLRC hRC = nullptr;
	};
	
	rendering_context* renderingContextInstance(window* win) {
		return new win32_rendering_context(win);
	}
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

int kengine::createDebugConsole()
{
	if (!AllocConsole())
		return 0;

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

int kengine::closeDebugConsole()
{
	return FreeConsole();
}

int kengine::osInitialize()
{
	return registerWindowClass(K_ENGINE_WINDOW_CLASS);
}

int kengine::osFinish()
{
	return unregisterWindowClass(K_ENGINE_WINDOW_CLASS);
}

void kengine::getDisplaySize(int* width, int* height)
{
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CYSCREEN);
}

int kengine::getDisplayCenterPosX(int displayWidth)
{
	return (GetSystemMetrics(SM_CXSCREEN) - displayWidth) / 2;
}

int kengine::getDisplayCenterPosY(int displayHeight)
{
	return (GetSystemMetrics(SM_CYSCREEN) - displayHeight) / 2;
}

int kengine::getNumberOfMonitors()
{
	return 0;
}

/*
	Get DPI (dots per inch) value for the specified window
*/
//#include <ShellScalingApi.h> // este include precisa linkar com shcore.lib
//unsigned int kengine::getDPI(const window* win)
//{
//	//HMONITOR hMonitor = MonitorFromWindow(hWindow, MONITOR_DEFAULTTONEAREST);
//	//UINT x;
//	//UINT y;
//	//HRESULT r = GetDpiForMonitor(hMonitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &x, &y);
//	return GetDpiForWindow(hWindow);
//}

void kengine::quitApplication(int returnCode)
{
	PostQuitMessage(returnCode);
}

void kengine::setGlobalUserEventsCallback(events_callback* evt)
{
	assert(globalUserEventsCallback == nullptr); // trying to override a globalUserEventsCallback
	globalUserEventsCallback = evt;
}

void kengine::handleSystemMessages()
{
	// (!) PLEASE DON'T INCLUDE I/O's STUFF HERE!
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			globalUserEventsCallback->onFinishEvent();
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void* kengine::getGLFunctionAddress(std::string name)
{
	/*
		References:
			- https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
	*/

	void* functionAddress = (void*)wglGetProcAddress(name.c_str());

	// some implementations can return these codes: 1, 2, 3 and -1
	if (functionAddress == nullptr || (functionAddress == (void*)0x01) || (functionAddress == (void*)0x02) || (functionAddress == (void*)0x03) || (functionAddress == (void*)-1)) {
		//HMODULE module = LoadLibraryA("opengl32.dll");
		//p = (void*)GetProcAddress(module, name);
		return nullptr;
	}

	return functionAddress;
}

int kengine::getAllGLProcedureAddress()
{
#pragma warning(disable: 4191)

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)getGLFunctionAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)getGLFunctionAddress("wglCreateContextAttribsARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)getGLFunctionAddress("wglSwapIntervalEXT");

#pragma warning(default: 4191)

	if (wglChoosePixelFormatARB == nullptr ||
		wglCreateContextAttribsARB == nullptr ||
		wglSwapIntervalEXT == nullptr) {
		return 0;
	}

	if (!getAllGLProcedures()) {
		return 0;
	}

	return 1;
}

namespace kengine
{
	/*
		window styles
			- splash (no caption, no resize, no move)
			- default (caption, sys menu, minimize box, maximize box, resize, can move)
			- minimal (caption, sys menu, no resize, can move)
			- minimal no sys menu (caption, no resize, can move)
	*/
//	enum class WINDOW_STYLE
//	{
//		SPLASH,
//		MINIMAL,
//		MINIMAL_NO_SYS,
//		WINDOWED_FULLSCREEN,
//		DEFAULT
//	};

//	enum WINDOW_ADDITIONAL_STYLE
//	{
//#if defined(_WIN32)
//		ACCEPT_FILES = WS_EX_ACCEPTFILES,
//		APPWINDOW = WS_EX_APPWINDOW,
//		COMPOSITED = WS_EX_COMPOSITED,
//		NOACTIVATE = WS_EX_NOACTIVATE,
//		TOOLWINDOW = WS_EX_TOOLWINDOW,
//		TOPMOST = WS_EX_TOPMOST
//#else
//		ACCEPT_FILES,
//		APPWINDOW,
//		COMPOSITED,
//		NOACTIVATE,
//		TOOLWINDOW,
//		TOPMOST
//#endif
//	};
}


/*
	This method specify the swap interval (synchronization of the front and back frame buffers with vertical blanks)
	@param value 1 enable vsync and value 0 disable vsync
*/
//int kengine::rendering_context::setVSync(int vsync)
//{
//	return wglSwapIntervalEXT(vsync);
//}


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

	if (RegisterClassEx(&windowClass) == 0) {
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

	if (!ret) {
		assert(!(globalUserEventsCallback == nullptr));
		DWORD error = GetLastError();
		globalUserEventsCallback->debugMessage("It was not possible to unregister a " + windowClassName + " class window: " + std::to_string(error));
	}

	return ret;
}

/*
	Win32 window procedure
*/
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	assert(!(globalUserEventsCallback == nullptr));

	if(!globalAppManager.isCallbackCallsEnabled())
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	globalUserEventsCallback->customWindowProcedure(&hWnd, &uMsg, &wParam, &lParam);

	switch (uMsg)
	{
	case WM_DESTROY: // 0x0002
		globalUserEventsCallback->onWindowDestroy();
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
		globalUserEventsCallback->closeButtonEvent();
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