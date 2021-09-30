/*
	K-Engine Win32 API Wrapper
	This file is part of the K-Engine.

	Copyright (C) 2021 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_WIN32_API_WRAPPER_CLASS_H
#define K_ENGINE_WIN32_API_WRAPPER_CLASS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <keeventhandler.h>

// ****************************************************************************
//  Win32 Window Class
// ****************************************************************************
#define WINDOWCLASSNAME LPCSTR("KWINDOWCLASS")

// ****************************************************************************
//  Win32 Window Procedure
// ****************************************************************************
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ****************************************************************************
//  K-Engine Win32 API Wrapper Class
// ****************************************************************************
class KEWINAPIWrapper
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructor
	// ------------------------------------------------------------------------
	KEWINAPIWrapper();
	~KEWINAPIWrapper();

	KEWINAPIWrapper(const KEWINAPIWrapper& win)
		: hWindow(NULL), hDC(NULL), hRC(NULL)
	{
		// (!) Tenha cuidado com chamada implícica do construtor de cópia.
		//     Pode ocorrer problemas se dois objetos apontarem para hWindow,
		//     hDC e hRC.
	}

	// ------------------------------------------------------------------------
	//  CPU's stuff
	// ------------------------------------------------------------------------
	long long getHighResolutionTimerCounter() const;
	long long getHighResolutionTimerFrequency() const;

	// ------------------------------------------------------------------------
	//  Window System's stuff
	// ------------------------------------------------------------------------
	int createWindow(int x, int y, int width, int height, std::string name, unsigned int style);
	int destroyWindow();
	int showWindow(int showType) const;

	// ------------------------------------------------------------------------
	//  Message Events Handling (Message Pump)
	// ------------------------------------------------------------------------
	void handleSystemMessages() const;

	// ------------------------------------------------------------------------
	//  OPENGL REDENRING's stuff
	// ------------------------------------------------------------------------
	int initializeRenderingSystem();
	int swapBuffers() const;

	// ------------------------------------------------------------------------
	//  Creating new Console for Debug
	// ------------------------------------------------------------------------
	int createDebugConsole() const;
	int closeDebugConsole() const;

	// ------------------------------------------------------------------------
	//  Set Global Event Handler
	// ------------------------------------------------------------------------
	void setGlobalEventHandler(KEEventHandler *eventHandler);

private:
	HWND hWindow;
	HDC hDC;
	HGLRC hRC;
};

#endif