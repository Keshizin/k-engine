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

// (ok compile lib)
// CL /c /EHsc core/src/kewindow.cpp core/src/kewinapiwrapper.cpp /Icore/inc
// CL /c /EHsc core/src/kewindow.cpp core/src/kewinapiwrapper.cpp core/src/ge.cpp core/src/getimehandler.cpp /Icore/inc
// lib kewindow.obj kewinapiwrapper.obj /OUT:kengine.lib
// lib kewindow.obj kewinapiwrapper.obj ge.obj getimehandler.obj /OUT:kengine.lib
// CL game/main.cpp /Icore/inc /EHsc /link kengine.lib gdi32.lib opengl32.lib user32.lib /OUT:game.exe

#include <kewindow.h>
#include <geaux.h>
// #define K_DEBUG 1

// #include <ge.h>
#include <gl/gl.h>

#include <iostream>

// ****************************************************************************
//  Game Engine Core Events
// ****************************************************************************
class GameEventHandler : public KEEventHandler
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

// KEngine* engine;

bool isLoop = true;

// ****************************************************************************
//  Point Entry Execution
// ****************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);

	GameEventHandler eventHandler;

	KEWINAPIWrapper winapiWrapper;
	winapiWrapper.setGlobalEventHandler(&eventHandler);
	winapiWrapper.createDebugConsole();

	KEWindow gameWindow(&winapiWrapper);

	gameWindow.setWindow(0, 0, 640, 480, "K-ENGINE TEST", K_WINDOW_COMPLETE);
	gameWindow.create();
	gameWindow.show(nCmdShow);

	while(isLoop)
	{
		winapiWrapper.handleSystemMessages();
	}

	winapiWrapper.closeDebugConsole();

	// engine = new KEngine(&eventHandler);

	// std::cout << "STARTING K-ENGINE DEMO" << std::endl;

	// // Setting up the window
	// engine->getGameWindow()->setWindow(20, 20, 800, 800, "K-ENGINE DEMO", K_WINDOW_COMPLETE);

	// // Create the window
	// engine->getGameWindow()->create();

	// // // Initializing OpenGL's context
	// // engine->getAPIWrapper()->initializeRenderingSystem();

	// // Show the window
	// engine->getGameWindow()->show(nCmdShow);

	// // glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

	// // // Starting the game loop
	// // engine->startMainLoop();

	// std::cout << "END K-ENGINE DEMO" << std::endl;
	// delete engine;

	// while(1);

	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GameEventHandler::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}

void GameEventHandler::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}

void GameEventHandler::keyboardEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}

void GameEventHandler::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
	K_UNREFERENCED_PARAMETER(width);
	K_UNREFERENCED_PARAMETER(height);
}

void GameEventHandler::finishAfterEvent()
{
	// engine->stopMainLoop();
	isLoop = false;
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