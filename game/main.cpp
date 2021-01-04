/*
	Game Engine Template
	This file provide a template for a game created with K-Engine.

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
#include <ge.h>
#include <gewinapiwrapper.h>

#include <GL/gl.h>
#include <GL/glu.h>

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

class GameEventHandler : public GEEventHandler
{
	void frameEvent();
	void mouseEvent(int button, int state, int x, int y);
	void mouseMotionEvent(int x, int y);
	void keyboardEvent(unsigned char key, int state);
	void keyboardSpecialEvent(unsigned char key, int state);
	void resizeWindowEvent(int width, int height);
	void finishAfterEvent();
	void finishBeforeEvent();
	void resumeEvent();
	void pauseEvent();
	void beforeMainLoopEvent();
};

KEngine *engine = 0;
GLfloat spin = 0;
bool isSpin = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameEventHandler gameEvents;
	engine = new KEngine(&gameEvents);

	// Setting up Game Window
	engine->getGameWindow()->setName("K-Engine!");
	engine->getGameWindow()->setWidth(GAME_WINDOW_WIDTH);
	engine->getGameWindow()->setHeight(GAME_WINDOW_HEIGHT);
	engine->getGameWindow()->setX(960 - (GAME_WINDOW_WIDTH / 2));
	engine->getGameWindow()->setY(540 - (GAME_WINDOW_HEIGHT / 2));
	engine->getGameWindow()->setStyle(K_WINDOW_DEFAULT);
	engine->getGameWindow()->create();
	engine->getGameWindow()->show(K_WINDOW_SHOW);

	// Setting up Rendering Engine
	engine->getRenderingSystem()->initialize();

	glViewport(0, 0, (GLsizei) GAME_WINDOW_WIDTH, (GLsizei) GAME_WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);

	std::cout << "> START GAME LOOP" << std::endl;

	engine->getRenderingSystem()->setVSync(0);
	engine->setFrameRate(220);
	engine->startMainLoop();

	std::cout << "> END GAME LOOP" << std::endl;
	
	delete engine;
	return 1;
}

void GameEventHandler::frameEvent()
{
	if(::isSpin)
	{
		::spin = ::spin + 2.0;

		if(::spin > 360.0)
		{
			::spin -= 360.0;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(::spin, 0.0f, 0.0f, 1.0f);
	glColor3f(1.0, 1.0, 1.0);
	glRectf(-25.0, -25.0, 25.0, 25.0);
	glPopMatrix();
	glFlush();
}

void GameEventHandler::mouseEvent(int button, int state, int x, int y)
{
	if(button == 0 && state == 1)
	{
		isSpin = true;
	}

	if(button == 1 && state == 1)
	{
		isSpin = false;
	}
}

void GameEventHandler::mouseMotionEvent(int x, int y)
{
}

void GameEventHandler::keyboardEvent(unsigned char key, int state)
{
	if(key == 27 && state == 1)
	{
		engine->getGameWindow()->destroy();
	}

	if(key == '1' && state == 1)
	{
		engine->getRenderingSystem()->setVSync(1);
	}
}

void GameEventHandler::keyboardSpecialEvent(unsigned char key, int state)
{
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
}