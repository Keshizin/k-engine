/*
	K-Engine Demonstration
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

#include <kecore.h>
#include <kewindow.h>
#include <kewinapiwrapper.h>
#include <keeventhandler.h>
#include <keaux.h>
#include <ketimer.h>

#include <gl/gl.h>
#include <gl/glu.h>
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
	void moveWindowEvent(int x, int y);
	void finishAfterEvent();
	void finishBeforeEvent();
	void resumeEvent();
	void pauseEvent();
	void beforeMainLoopEvent();
	void afterMainLoopEvent();
	void createWindowEvent();
};

KEngine* engine;

// ****************************************************************************
//  Point Entry Execution
// ****************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);

	GameEventHandler eventHandler;
	engine = new KEngine(&eventHandler);
	
	// Setting up the window
	engine->getGameWindow()->setWindow(50, 50, 450, 450, "K-ENGINE DEMO", K_WINDOW_COMPLETE);

	// Creating the window
	engine->getGameWindow()->create();

	// Initializing OpenGL's context
	engine->getAPIWrapper()->initializeRenderingSystem();

	// Showing the window
	engine->getGameWindow()->show(nCmdShow);

	// Starting the game loop
	engine->startMainLoop();

	delete engine;
	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);

	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	// ------------------------------

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	
	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	// ------------------------------
	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);

	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	// ------------------------------
	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);

	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	// ------------------------------
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	// ------------------------------
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);
	
	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);

	glEnd();
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

	// if(key == 27 && state)
	// {
	// 	engine->getGameWindow()->destroy();
	// }
}

void GameEventHandler::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
	// K_UNREFERENCED_PARAMETER(width);
	// K_UNREFERENCED_PARAMETER(height);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect;

	if(width <= height)
	{
		aspect = height / width;
	}
	else
	{
		aspect = width / height;
	}

	glOrtho(-65.0, 65.0, -65.0, 65.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(40, 60, 100, 0, 0, 0, 0, 1, 0);
}

void GameEventHandler::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}

void GameEventHandler::finishAfterEvent()
{
	engine->stopMainLoop();
}

void GameEventHandler::finishBeforeEvent()
{
	engine->getGameWindow()->destroy();
}

void GameEventHandler::resumeEvent()
{
}

void GameEventHandler::pauseEvent()
{
}

void GameEventHandler::beforeMainLoopEvent()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(2.0);

	int width = 450;
	int height = 450;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect;

	if(width <= height)
	{
		aspect = height / width;
	}
	else
	{
		aspect = width / height;
	}

	glOrtho(-65.0, 65.0, -65.0, 65.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(90, 60, 100, 0, 0, 0, 0, 1, 0);
}

void GameEventHandler::afterMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}