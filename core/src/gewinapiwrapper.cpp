/*
	Game Engine Win32 API Wrapper
	This file is part of the K-Engine.

	Copyright (C) 2020 Fabio Takeshi Ishikawa

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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>

#include <gewinapiwrapper.h>
#include <gewindow.h>

// ****************************************************************************
//  GEWINAPIWrapper Class - Constructors and Destructor
// ****************************************************************************
GEWINAPIWrapper::GEWINAPIWrapper()
{
	hWindow = NULL;
}

GEWINAPIWrapper::~GEWINAPIWrapper()
{
}

// ****************************************************************************
//  GEWINAPIWrapper Class - Public Methods
// ****************************************************************************
int GEWINAPIWrapper::createWindow(int x, int y, int width, int height, std::string name, unsigned int style)
{
	DWORD error;
	WNDCLASSEX windowClass;

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

	if(!RegisterClassEx(&windowClass))
	{
		// (!) Gravar o erro no componente de LOG
		error = GetLastError();
		std::cout << "(!) ERROR - It was not possible to register a class window: " << error << "\n" << std::endl;
		return 0;
	}

	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = 0;

	switch(style)
	{
		case K_WINDOW_SPLASH:
			dwStyle = WS_POPUP | WS_BORDER;
			break;

		case K_WINDOW_DEFAULT:
			dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU;
			break;

		case K_WINDOW_DEFAULT_NO_SYS:
			dwStyle = WS_POPUP | WS_BORDER | WS_CAPTION;
			break;

		case K_WINDOW_WINDOWED_FULLSCREEN:
			dwStyle = WS_POPUP | WS_BORDER | WS_MAXIMIZE;
			break;

		case K_WINDOW_COMPLETE:
			dwStyle = WS_POPUP | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
			break;
	}

	RECT windowSize;
	windowSize.left = (LONG)0;
	windowSize.right = (LONG)width;
	windowSize.top = (LONG)0;
	windowSize.bottom = (LONG)height;

	AdjustWindowRectEx(&windowSize, dwStyle, FALSE, dwExStyle);

	hWindow = CreateWindowEx(
		dwExStyle,
		LPCSTR(WINDOWCLASSNAME),
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

	if(hWindow == NULL)
	{
		error = GetLastError();
		std::cout << "(!) ERROR - It was not possible to create the window: " << error << "\n" << std::endl;

		int ret = UnregisterClass(WINDOWCLASSNAME, GetModuleHandle(NULL));

		if(ret == 0)
		{
			error = GetLastError();
			std::cout << "(!) ERROR - It was not possible to unregister a class window: " << error << "\n" << std::endl;
		}

		return 0;
	}
	
	return 1;
}

int GEWINAPIWrapper::destroyWindow()
{
	int ret;
	int err = 1;

	ret = DestroyWindow(hWindow);

	if(ret == 0)
	{
		// (!) Gravar o erro no componente de LOG
		DWORD error = GetLastError();
		std::cout << "(!) ERROR - It was not possible to destroy a window application: " << error << "\n" << std::endl;
		err = 0;
	}

	hWindow = NULL;

	ret = UnregisterClass(WINDOWCLASSNAME, GetModuleHandle(NULL));

	if(ret == 0)
	{
		DWORD error = GetLastError();
		std::cout << "(!) ERROR - It was not possible to unregister a class window: " << error << "\n" << std::endl;
		err = 0;
	}

	return err;
}

int GEWINAPIWrapper::showWindow(int showType)
{
	if(hWindow == NULL)
	{
		std::cout << "(!) ERROR - It was not possible to show the window: window handle is NULL\n" << std::endl;
	}

	return ShowWindow(hWindow, showType);
}

// ****************************************************************************
//  Win32 Window Procedure Definition
// ****************************************************************************
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}