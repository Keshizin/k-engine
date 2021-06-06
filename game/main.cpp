/*
	Game Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <geaux.h>

#include <gewinapiwrapper.h>

// ****************************************************************************
//  Game Engine Core Events
// ****************************************************************************
class GameEventHandler : public GEEventHandler
{
public:
	void frameEvent(double frameTime);
	void mouseEvent(int button, int state, int x, int y);
	void mouseMotionEvent(int x, int y);
	void keyboardEvent(unsigned long long key, int state) ;
	void keyboardSpecialEvent(unsigned long long key, int state);
	void resizeWindowEvent(int width, int height);
	void finishAfterEvent();
	void finishBeforeEvent();
	void resumeEvent();
	void pauseEvent();
	void beforeMainLoopEvent();
	void createWindowEvent();
};

// ****************************************************************************
//  Point Entry Execution
// ****************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);
	K_UNREFERENCED_PARAMETER(nCmdShow);

	GEWINAPIWrapper apiWrapper;
	GameEventHandler eventHandler;

	apiWrapper.setGlobalEventHandler(&eventHandler);
	apiWrapper.createDebugConsole();
	apiWrapper.createWindow(0, 0, 640, 480, "K-ENGINE!", 5);
	apiWrapper.showWindow(nCmdShow);
	apiWrapper.destroyWindow();

	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
}

void GameEventHandler::mouseEvent(int button, int state, int x, int y)
{
}

void GameEventHandler::mouseMotionEvent(int x, int y)
{
}

void GameEventHandler::keyboardEvent(unsigned long long key, int state)
{
}

void GameEventHandler::keyboardSpecialEvent(unsigned long long key, int state)
{
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
}

void GameEventHandler::finishAfterEvent()
{
}

void GameEventHandler::finishBeforeEvent()
{
}

void GameEventHandler::resumeEvent()
{
}

void GameEventHandler::pauseEvent()
{
}

void GameEventHandler::beforeMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}