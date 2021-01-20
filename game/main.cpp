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

#define WINDOW_LEFT   -1.0
#define WINDOW_RIGHT   1.0
#define WINDOW_BOTTOM -1.0
#define WINDOW_TOP     1.0

GLfloat angle = 0.0;

VERTEX v[4] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f
};

COLOR c[4] = {
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

FACE f[2] = {
	3, {0, 1, 2},
	3, {1, 3, 2}
};

float rect_vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f
};

float rect_colors[] = {
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

unsigned int rect_indices[] = {0, 1, 2, 1, 3, 2};

class GameEventHandler : public GEEventHandler
{
	void frameEvent();
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

GEModel model;

KEngine *engine = 0;
GETimer *timer = 0;

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
	engine->getRenderingSystem()->setWindowAspectCorrectionState(true);

	engine->getRenderingSystem()->setWindow(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP);
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
	return 1;
}

PFNGLGENBUFFERSPROC glGenBuffers1;
PFNGLISBUFFERPROC glIsBuffer1;
PFNGLBINDBUFFERPROC glBindBuffer1;
PFNGLBUFFERDATAPROC glBufferData1;
PFNGLBUFFERSUBDATAPROC glBufferSubData1;
PFNGLMAPBUFFERPROC glMapBuffer1;
PFNGLUNMAPBUFFERPROC glUnmapBuffer1;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange1;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange1;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData1;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers1;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays1;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray1;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays1;

void GameEventHandler::frameEvent()
{
	angle++;

	if(angle > 360.0f)
	{
		angle = 0.0f;
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glRotatef(angle, 0.0, 0.0, 1.0);	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	if(key == 27 && state == 1)
	{
		engine->getGameWindow()->destroy();
	}

	if(key == '1' && state == 1)
	{
		timer->startLoop(0);
	}
}

void GameEventHandler::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}

void GameEventHandler::resizeWindowEvent(int width, int height)
{
	engine->getRenderingSystem()->setViewport(0, 0, width, height);
	engine->getRenderingSystem()->setProjection();

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
	for(int i = 0; i < 4; i++)
	{
		std::cout << "x: " << v[i].x << std::endl;
		std::cout << "y: " << v[i].y << std::endl;
		std::cout << "z: " << v[i].z << std::endl;
	}

	// glClearColor(247.0f / 255.0f, 194.0f / 255.0f, 23.0f / 255.0f, 1.0f);
	glClearColor(44.0f / 255.0f, 0.0f / 255.0f, 30.0f / 255.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// unsigned int vao = engine->getRenderingSystem()->loadModel(rect_vertices, rect_colors, rect_indices);

	glGenBuffers1 = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glIsBuffer1 = (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
	glBindBuffer1 = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData1 = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData1 = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glMapBuffer1 = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBuffer1 = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glMapBufferRange1 = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
	glFlushMappedBufferRange1 = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)wglGetProcAddress("glFlushMappedBufferRange");
	glCopyBufferSubData1 = (PFNGLCOPYBUFFERSUBDATAPROC)wglGetProcAddress("glCopyBufferSubData");
	glDeleteBuffers1 = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffers");
	glGenVertexArrays1 = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray1 = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays1 = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");

	GLuint vao;
	GLuint bo[3] = {0};

	glGenVertexArrays1(1, &vao);
	glBindVertexArray1(vao);

	if(rect_vertices)
	{
		glGenBuffers1(1, &bo[0]);
		glBindBuffer1(GL_ARRAY_BUFFER, bo[0]);
		glBufferData1(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY); // (remove)
		glVertexPointer(3, GL_FLOAT, 0, 0); // (remove)
	}

	if(rect_colors)
	{
		glGenBuffers1(1, &bo[1]);
		glBindBuffer1(GL_ARRAY_BUFFER, bo[1]);
		glBufferData1(GL_ARRAY_BUFFER, sizeof(rect_colors), rect_colors, GL_STATIC_DRAW);

		glEnableClientState(GL_COLOR_ARRAY); // (remove)
		glColorPointer(3, GL_FLOAT, 0, 0); // (remove)
	}

	if(rect_indices)
	{
		glGenBuffers1(1, &bo[2]);
		glBindBuffer1(GL_ELEMENT_ARRAY_BUFFER, bo[2]);
		glBufferData1(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);
	}
}

void GameEventHandler::createWindowEvent()
{
}