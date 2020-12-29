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

#ifndef K_ENGINE_WIN_API_WRAPPER_CLASS_H
#define K_ENGINE_WIN_API_WRAPPER_CLASS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <geapiwrapper.h>
#include <geeventhandler.h>

// ****************************************************************************
//  Win32 Window Class
// ****************************************************************************
#define WINDOWCLASSNAME LPCSTR("KWINDOWCLASS")

// ****************************************************************************
//  Win32 Window Procedure
// ****************************************************************************
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ****************************************************************************
//  Game Engine Win32 API Wrapper
// ****************************************************************************
class GEWINAPIWrapper : public GEAPIWrapper
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructor
	// ------------------------------------------------------------------------
	GEWINAPIWrapper();
	~GEWINAPIWrapper();

	// ------------------------------------------------------------------------
	//  Window System's stuff
	// ------------------------------------------------------------------------
	int createWindow(int x, int y, int width, int height, std::string name, unsigned int style);
	int destroyWindow();
	int showWindow(int showType);

	// ------------------------------------------------------------------------
	// Message Events Handling (Message Pump)
	// ------------------------------------------------------------------------
	void handleSystemMessages();
	
	void setGlobalEventHandler(GEEventHandler *eventHandler);

private:
	HWND hWindow;

};

#endif