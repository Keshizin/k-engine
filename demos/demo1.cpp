/*
	Game Engine Demonstration
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

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

#include <ge.h>
#include <geaux.h>

#include <iostream>

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

#define WINDOW_LEFT    -5.0
#define WINDOW_RIGHT    5.0
#define WINDOW_BOTTOM  -5.0
#define WINDOW_TOP      5.0

class GameEventHandler : public GEEventHandler
{
	void frameEvent(double frameTime);
	void mouseEvent(int button, int state, int x, int y);
	void mouseMotionEvent(int x, int y);
	void keyboardEvent(unsigned long long key, int state);
	void keyboardSpecialEvent(unsigned long long key, int state);
	void resizeWindowEvent(int width, int height);
	void finishAfterEvent();
	void finishBeforeEvent();
	void resumeEvent();
	void pauseEvent();
	void beforeMainLoopEvent();
	void createWindowEvent();
};

KEngine *engine = 0;
GETimer *timer = 0;
GEEntity *entity = 0;

// ****************************************************************************
//  Point Entry Execution
// ****************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);

	GameEventHandler gameEvents;
	engine = new KEngine(&gameEvents);
	timer = new GETimer(engine->getTimeHandler());

	// Setting up Game Window
	engine->getGameWindow()->setName("K-Engine!");
	engine->getGameWindow()->setWidth(GAME_WINDOW_WIDTH);
	engine->getGameWindow()->setHeight(GAME_WINDOW_HEIGHT);
	engine->getGameWindow()->setX(960 - (GAME_WINDOW_WIDTH / 2));
	engine->getGameWindow()->setY(540 - (GAME_WINDOW_HEIGHT / 2));
	engine->getGameWindow()->setStyle(K_WINDOW_COMPLETE);
	engine->getGameWindow()->create();

	// Setting up Rendering Engine
	engine->getRenderingSystem()->initialize();
	engine->getRenderingSystem()->setRenderingContext(K_CONTEXT_2D);
	engine->getRenderingSystem()->setWindowAspectCorrectionState(false);

	engine->getRenderingSystem()->setRenderingWindow(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP);
	engine->getRenderingSystem()->setViewport(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

	engine->getRenderingSystem()->setProjection();
	engine->getRenderingSystem()->setVSync(0);
	engine->getRenderingSystem()->getSystemVersion();

	engine->setFrameRate(0);
	engine->getGameWindow()->show(nCmdShow);
	timer->setTimerInMs(1000);
	engine->startMainLoop();

	delete engine;
	delete timer;
	delete entity;
	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	engine->getRenderingSystem()->drawGlobaldAxis();
	entity->update(frameTime);
	entity->draw();
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
	std::cout << "@DEBUG | keyboardEvent | key: " << key << " - state: " << state << std::endl;

	if(key == 27 && state == 1)
	{
		engine->getGameWindow()->destroy();
	}

	if(key == '1' && state == 1)
	{
		// zoom in
		double zoom = engine->getRenderingSystem()->getZoom();
		engine->getRenderingSystem()->setZoom(zoom + 1.0f);
		engine->getRenderingSystem()->setProjection();
	}

	if(key == '2' && state == 1)
	{
		// zoom out
		double zoom = engine->getRenderingSystem()->getZoom();
		engine->getRenderingSystem()->setZoom(zoom - 1.0f);
		engine->getRenderingSystem()->setProjection();
	}

	if(key == 37 && state == 1)
	{
		double offset = engine->getRenderingSystem()->getRenderingWindowOffsetX();
		engine->getRenderingSystem()->setRenderingWindowOffsetX(offset - 0.1);
		engine->getRenderingSystem()->setProjection();

	}

	if(key == 38 && state == 1)
	{
		double offset = engine->getRenderingSystem()->getRenderingWindowOffsetY();
		engine->getRenderingSystem()->setRenderingWindowOffsetY(offset + 0.1);
		engine->getRenderingSystem()->setProjection();
	}

	if(key == 39 && state == 1)
	{
		double offset = engine->getRenderingSystem()->getRenderingWindowOffsetX();
		engine->getRenderingSystem()->setRenderingWindowOffsetX(offset + 0.1);
		engine->getRenderingSystem()->setProjection();
	}

	if(key == 40 && state == 1)
	{
		double offset = engine->getRenderingSystem()->getRenderingWindowOffsetY();
		engine->getRenderingSystem()->setRenderingWindowOffsetY(offset - 0.1);
		engine->getRenderingSystem()->setProjection();
	}
}

void GameEventHandler::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
	engine->getRenderingSystem()->setRenderingWindow(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP);
	engine->getRenderingSystem()->setViewport(0, 0, width, height);
	engine->getRenderingSystem()->setProjection();

	if(entity)
	{
		GERECT w = engine->getRenderingSystem()->getRenderingWindow();
		entity->setBounding(w.left, w.right, w.top, w.bottom);
	}

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
	// glClearColor(247.0f / 255.0f, 194.0f / 255.0f, 23.0f / 255.0f, 1.0f);
	glClearColor(44.0f / 255.0f, 0.0f / 255.0f, 30.0f / 255.0f, 1.0f);
	engine->getRenderingSystem()->resetView();

	MODEL *m = new MODEL();

	m->vertices = new VERTEX[4];

	m->vertices[0].x = WINDOW_LEFT - 5.0f;
	m->vertices[0].y = -0.5f;
	m->vertices[0].z = 0.0f;

	m->vertices[1].x = WINDOW_RIGHT + 5.0f;
	m->vertices[1].y = -0.5f;
	m->vertices[1].z = 0.0f;

	m->vertices[2].x = WINDOW_LEFT - 5.0f;
	m->vertices[2].y = 0.5f;
	m->vertices[2].z = 0.0f;

	m->vertices[3].x = WINDOW_RIGHT + 5.0f;
	m->vertices[3].y = 0.5f;
	m->vertices[3].z = 0.0f;

	m->colors = new COLOR[4];

	m->colors[0].r = 0.0f;
	m->colors[0].g = 0.0f;
	m->colors[0].b = 0.0f;
	m->colors[1].r = 1.0f;
	m->colors[1].g = 1.0f;
	m->colors[1].b = 1.0f;
	m->colors[2].r = 0.0f;
	m->colors[2].g = 0.0f;
	m->colors[2].b = 0.0f;
	m->colors[3].r = 1.0f;
	m->colors[3].g = 1.0f;
	m->colors[3].b = 1.0f;

	m->indices = new unsigned int[6];
	int k = 0;

	for(int i = 0; i < 1; i++)
	{
		m->indices[k++] = 0;
		m->indices[k++] = 1;
		m->indices[k++] = 2;
		m->indices[k++] = 1;
		m->indices[k++] = 3;
		m->indices[k++] = 2;
	}

	m->total_indices = 6;
	m->total_vertex = 4;

	entity = new GEEntity(m);
	entity->setSpeed(0.0f, 0.0f, 0.0f);
	//GERECT w = engine->getRenderingSystem()->getRenderingWindow();
	//entity->setBounding(w.left, w.right, w.top, w.bottom);
	entity->setBounding(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_TOP, WINDOW_BOTTOM);
	entity->setRotate(0.0f, 0.0f, 0.0f, 1.0f);
	entity->setScale(1.0f, 1.0f, 1.0f);
	entity->setTranslate(0.0f, 0.0f, 0.0f);
	
}

void GameEventHandler::createWindowEvent()
{
}