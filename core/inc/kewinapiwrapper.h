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

#ifndef K_ENGINE_WIN32_API_WRAPPER_H
#define K_ENGINE_WIN32_API_WRAPPER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

#define WINDOWCLASSNAME LPCSTR("KWINDOWCLASS")

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace kengine
{
	// ------------------------------------------------------------------------
	//  CPU's stuff
	// ------------------------------------------------------------------------
	long long getHighResolutionTimerCounter();
	long long getHighResolutionTimerFrequency();


	// ------------------------------------------------------------------------
	//  message events handling (message pump)
	// ------------------------------------------------------------------------
	void handleSystemMessages();
	

	// --------------------------------------------------------------------
	//  set global event handler
	// --------------------------------------------------------------------
	class eventhandler;
	void setGlobalEventHandler(kengine::eventhandler* evt);


	// ------------------------------------------------------------------------
	//  creating new console for debug
	// ------------------------------------------------------------------------
	int createDebugConsole();
	int closeDebugConsole();


	// ------------------------------------------------------------------------
	//  kengine win32 window class
	// ------------------------------------------------------------------------
	class win32wrapper
	{
	public:
		win32wrapper();
		~win32wrapper();

		win32wrapper(const win32wrapper& copy) = delete; // copy constructor
		win32wrapper& operator=(const win32wrapper& copy) = delete; // copy assignment
		win32wrapper(win32wrapper&& move) noexcept = delete;  // move constructor

		// win32 window's stuff
		int create(int x, int y, int width, int height, std::string name, unsigned int style);
		int destroy();
		int show(int showType) const;

		//  opengl rendering's stuff
		int initializeRenderingSystem();
		int swapBuffers() const;

	private:
		HWND hWindow;
		HDC hDC;
		HGLRC hRC;
	};
}

#endif