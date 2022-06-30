/*
	K-Engine Win32 API Wrapper
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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

#include <kewinapiwrapper.h>
#include <keconstants.h>
#include <keeventhandler.h>
#include <keaux.h>

#include <tchar.h>
#include <iostream>

static kengine::eventhandler* globalEventHandler = 0;


// ----------------------------------------------------------------------------
//  CPU's stuff
// ----------------------------------------------------------------------------
long long kengine::getHighResolutionTimerCounter()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}


long long kengine::getHighResolutionTimerFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}


// ----------------------------------------------------------------------------
//  message events handling (message pump)
// ----------------------------------------------------------------------------
void kengine::handleSystemMessages()
{
	// (!) PLEASE DONT INCLUDE I/O's STUFF HERE!
	MSG msg;
	
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			globalEventHandler->finishAfterEvent();
		}
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


// ----------------------------------------------------------------------------
//  set global event handler
// ----------------------------------------------------------------------------
void kengine::setGlobalEventHandler(eventhandler *evt)
{
	globalEventHandler = evt;
}


// ----------------------------------------------------------------------------
//  creating new console for debug
// ----------------------------------------------------------------------------
int kengine::createDebugConsole()
{
	if(!AllocConsole())
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


int kengine::closeDebugConsole()
{
	return FreeConsole();
}


// ----------------------------------------------------------------------------
//  kengine win32window class
// ----------------------------------------------------------------------------
kengine::win32wrapper::win32wrapper()
	: hWindow{ nullptr }, hDC{ nullptr }, hRC{ nullptr }
{
}


kengine::win32wrapper::~win32wrapper()
{
}


int kengine::win32wrapper::create(int x, int y, int width, int height, std::string name, unsigned int style)
{
	//DWORD error;
	WNDCLASSEX windowClass = { 0 };

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
	windowClass.lpszClassName = WINDOWCLASSNAME; // mandatory

	if (RegisterClassEx(&windowClass) == 0)
	{
		// (!) Write the error to the LOG Component
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to register a class window: " << error << "\n")
		return 0;
	}

	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = 0;

	switch (style)
	{
	case K_WINDOW_SPLASH:
		dwStyle = WS_POPUP | WS_BORDER;
		break;

	case K_WINDOW_DEFAULT:
		dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU;
		break;

	case K_WINDOW_NO_SYS:
		dwStyle = WS_POPUP | WS_BORDER | WS_CAPTION;
		break;

	case K_WINDOW_WINDOWED_FULLSCREEN:
		dwStyle = WS_POPUP | WS_BORDER | WS_MAXIMIZE;
		break;

	case K_WINDOW_COMPLETE:
		dwStyle = WS_POPUP | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
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
		WINDOWCLASSNAME,
		LPCSTR(name.c_str()),
		dwStyle,
		x,
		y,
		windowSize.right - windowSize.left,
		windowSize.bottom - windowSize.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (hWindow == NULL)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create the window: " << error << "\n")

		BOOL ret = UnregisterClass(WINDOWCLASSNAME, GetModuleHandle(NULL));

		if (!ret)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister a class window: " << error << "\n")
		}

		return 0;
	}

	return 1;
}


int kengine::win32wrapper::destroy()
{
	BOOL ret;
	int isSuccessful = 1;

	if (hRC != NULL)
	{
		ret = wglMakeCurrent(NULL, NULL);

		if (ret == FALSE)
		{
			DWORD error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the rendering context: " << error << "\n")
			error = 0;
		}

		ret = wglDeleteContext(hRC);

		if (ret == FALSE)
		{
			DWORD error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to delete the rendering context: " << error << "\n")
			error = 0;
		}

		hRC = NULL;
	}

	if (hDC != NULL)
	{
		ret = ReleaseDC(hWindow, hDC);

		if (!ret)
		{
			DWORD error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the device context: " << error << "\n")
			error = 0;
		}

		hDC = NULL;
	}

	ret = DestroyWindow(hWindow);

	if (!ret)
	{
		// (!) Write the error to the LOG Component
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to destroy a window application: " << error << "\n")
		isSuccessful = 0;
	}

	hWindow = NULL;

	ret = UnregisterClass(WINDOWCLASSNAME, GetModuleHandle(NULL));

	if (!ret)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister a class window: " << error << "\n")
		isSuccessful = 0;
	}

	return isSuccessful;
}


int kengine::win32wrapper::show(int showType) const
{
	return ShowWindow(hWindow, showType);
}


//  opengl rendering's stuff
int kengine::win32wrapper::initializeRenderingSystem()
{
	int ret;
	
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24, // cColorBits
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
		0, // cStencilBits
		0, // cAuxBuffers
		PFD_MAIN_PLANE, // iLayerType
		0, // bReserved
		0, // dwLayerMask
		0, // dwVisibleMask
		0 // dwDamageMask
	};

	hDC = GetDC(hWindow);

	if(hDC == NULL)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to get device context: " << error << "\n")
		return 0;
	}

	int PixelFormat = ChoosePixelFormat(hDC, &pfd);

	if(!PixelFormat)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to choose an pixel format: " << error << "\n")

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the device context : " << error << "\n")
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to destroy the window: " << error << "\n")
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister the window class: " << error << "\n")
		}

		return 0;
	}

	ret = SetPixelFormat(hDC, PixelFormat, &pfd);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to set the format pixel: " << error << "\n")

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the device context: " << error << "\n")
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to destroy the window: " << error << "\n")
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister the window class: " << error << "\n")
		}

		return 0;
	}

	hRC = wglCreateContext(hDC);

	if(hRC == NULL)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create the rendering context: " << error << "\n")

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the device context: " << error << "\n")
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to destroy the window: " << error << "\n")
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister the window class: " << error << "\n")
		}

		return 0;
	}

	ret = wglMakeCurrent(hDC, hRC);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to make current the rendering context: " << error << "\n")

		ret = wglDeleteContext(hRC);

		if(ret == FALSE)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to delete the rendering context: " << error << "\n")
		}

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to release the device context: " << error << "\n")
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to destroy the window: " << error << "\n")
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			error = GetLastError();
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to unregister the window class: " << error << "\n")
		}

		return 0;
	}

	return 1;
}


int kengine::win32wrapper::swapBuffers() const
{
	return SwapBuffers(hDC);
}


// ----------------------------------------------------------------------------
//  Win32 Window Procedure Definition
// ----------------------------------------------------------------------------
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// prevent the null pointer exception!
	if(!globalEventHandler)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	// --------------------------------------------------------------------
	//  WINDOW MESSAGES
	// --------------------------------------------------------------------
	// case WM_GETMINMAXINFO:
		// Informações a respeito da janela (importante para múltiplos monitores)
		// break;

	// case WM_NCCREATE:
		// break;

	// case WM_NCCALCSIZE:
		// break;

	case WM_CREATE:
		globalEventHandler->createWindowEvent();
		break;

	case WM_SIZE:
		globalEventHandler->resizeWindowEvent(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_MOVE:
		globalEventHandler->moveWindowEvent(LOWORD(lParam), HIWORD(lParam));
		break;

	// case WM_SHOWWINDOW:
		// break;

	//case WM_WINDOWPOSCHANGING:
		// break;

	// case WM_ACTIVATEAPP:
		// Enviado quando uma janela pertencente a um aplicativo diferente da
		// janela ativa está prestes a ser ativada. A mensagem é enviada para
		// o aplicativo cuja janela está sendo ativada e para o aplicativo
		// cuja janela está sendo desativada.
		// break;

	// case WM_NCACTIVATE:
		// break;

	// case WM_GETICON:
		// break;

	// case WM_WINDOWPOSCHANGED:
		// break;

	case WM_CLOSE:
		globalEventHandler->finishBeforeEvent();
		break;

	case WM_DESTROY:
		// Evento capturado depois da chamada de DestroyWindow e antes de seu término de execução
		// Ela é enviada após a janela ser removida da tela.
		PostQuitMessage(0);
		break;

	//case WM_NCDESTROY:
		// break;

	// --------------------------------------------------------------------
	//  INPUT QUEUE MESSAGES
	// --------------------------------------------------------------------
	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE)
			globalEventHandler->resumeEvent();
		else if (wParam == WA_CLICKACTIVE)
			globalEventHandler->resumeEvent();
		else if (wParam == WA_INACTIVE)
			globalEventHandler->pauseEvent();

		break;

	// case WM_IME_SETCONTEXT:
		// break;

	// case WM_IME_NOTIFY:
		// break;

	// case WM_SETFOCUS:
		// break;

	// case WM_KILLFOCUS:
		// break;

	// ********************************************************************
	//  MOUSE MESSAGES
	// ********************************************************************
	// case WM_NCHITTEST:
		// break;

	// case WM_MOUSEFIRST:
		// break;

	// case WM_NCMOUSEMOVE:
		// break;

	// case WM_NCLBUTTONDOWN:
	 	// break;

	// case WM_CAPTURECHANGED:
		// break;

	// case WM_SETCURSOR:
		// break;

	case WM_MOUSEMOVE:
		globalEventHandler->mouseMotionEvent(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		globalEventHandler->mouseEvent(K_MOUSE_LEFT_BUTTON, K_MOUSE_DOWN, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONUP:
		globalEventHandler->mouseEvent(K_MOUSE_LEFT_BUTTON, K_MOUSE_UP, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_MBUTTONDOWN:
		globalEventHandler->mouseEvent(K_MOUSE_MIDDLE_BUTTON, K_MOUSE_DOWN, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_MBUTTONUP:
		globalEventHandler->mouseEvent(K_MOUSE_MIDDLE_BUTTON, K_MOUSE_UP, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_RBUTTONDOWN:
		globalEventHandler->mouseEvent(K_MOUSE_RIGHT_BUTTON, K_MOUSE_DOWN, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_RBUTTONUP:
		globalEventHandler->mouseEvent(K_MOUSE_RIGHT_BUTTON, K_MOUSE_UP, LOWORD(lParam), HIWORD(lParam));
		break;

	// case WM_NCMOUSELEAVE:
		// break;

	// ********************************************************************
	//  KEYBOARD MESSAGES
	// ********************************************************************
	case WM_SYSKEYDOWN:
		globalEventHandler->keyboardSpecialEvent(wParam, 1);
		break;

	case WM_SYSKEYUP:
		globalEventHandler->keyboardSpecialEvent(wParam, 0);
		break;

	case WM_KEYDOWN:
		globalEventHandler->keyboardEvent(wParam, 1);
		break;

	case WM_KEYUP:
		globalEventHandler->keyboardEvent(wParam, 0);
		break;

	// case WM_CHAR:
		// break;

	// case WM_SYSCOMMAND:
		// break;

	// --------------------------------------------------------------------
	//  PAINTING AND DRAWING MESSAGES
	// --------------------------------------------------------------------
	// case WM_NCPAINT:
		// break;

	// case WM_PAINT:
		// break;

	// case WM_ERASEBKGND:
		// break;

	// --------------------------------------------------------------------
	//  DESKTOP WINDOW MANAGER (DWM) MESSAGES
	// --------------------------------------------------------------------
	// case WM_DWMNCRENDERINGCHANGED:
		// break;
		
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}