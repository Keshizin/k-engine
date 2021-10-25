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
#include <kerenderingsystem.h>
#include <keimage.h>
#include <kemodel.h>
#include <kelight.h>
#include <keobject.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

#include <iostream>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800
#define TOTAL_OBJ     15
#define SENS_ROTATE   5.0
#define SENSE_CAMERA  10.0

double rotateX = 0.0;
double rotateY = 0.0;
double rotateZ = 0.0;
double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 1.5;

int render_type = 1;

int init_x, init_y;
double init_rotateX, init_rotateY;
double init_cameraX, init_cameraY, init_cameraZ;

int buttonState = -1;

KEngine* engine;
KERenderingSystem* renderingSystem;

DIB* img = 0;

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

	renderingSystem = new KERenderingSystem(engine->getAPIWrapper());
	renderingSystem->setRenderingContext(K_CONTEXT_2D);
	renderingSystem->setRenderingWindow(-2, 2, -2, 2);
	renderingSystem->setProjectionZNear(0.1);
	renderingSystem->setProjectionZFar(500.0);
	renderingSystem->setProjectionFOVY(90.0);
	renderingSystem->setWindowAspectCorrectionState(true);

	// Setting up the window
	engine->getGameWindow()->setWindow(1000, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "K-ENGINE DEMO", K_WINDOW_COMPLETE);

	// Creating the window
	engine->getGameWindow()->create();

	renderingSystem->initialize();

	// Showing the window
	engine->getGameWindow()->show(nCmdShow);

	// Starting the game loop
	engine->startMainLoop();

	delete img;
	delete renderingSystem;
	delete engine;
	return 1;
}

float final = 1.0;
int filtro = GL_NEAREST;

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1, 0, 0);

	glNormal3f(0,0,1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-1,-1,0);
		glTexCoord2f(final,0);
		glVertex3f( 1,-1,0);
		glTexCoord2f(final,final);
		glVertex3f( 1, 1,0);
		glTexCoord2f(0,final);
		glVertex3f(-1, 1,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void GameEventHandler::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
	
	if(state == 1)
	{
		init_x = x;
		init_y = y;
		init_rotateX = rotateX;
		init_rotateY = rotateY;
		init_cameraX = cameraX;
		init_cameraY = cameraY;
		init_cameraZ = cameraZ;
		buttonState = button;
	}

	if(state == 0)
	{
		buttonState = -1;
	}
}

void GameEventHandler::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);

	if(buttonState == K_MOUSE_LEFT_BUTTON)
	{
		rotateY = init_rotateY - ((init_x - x) / SENS_ROTATE);
		rotateX = init_rotateX - ((init_y - y) / SENS_ROTATE);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

	if(buttonState == K_MOUSE_MIDDLE_BUTTON)
	{
		cameraZ = init_cameraZ + ((init_y - y) / SENSE_CAMERA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

	if(buttonState == K_MOUSE_RIGHT_BUTTON)
	{
		cameraX = init_cameraX + ((init_x - x) / SENSE_CAMERA);
		cameraY = init_cameraY - ((init_y - y) / SENSE_CAMERA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}
}

void GameEventHandler::keyboardEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);

	if(key == '1' && state)
	{
		if(final <= 10.0)
			final += 0.1;

		std::cout << "> keyboardEvent: " << final << std::endl;
	}

	if(key == '2' && state)
	{
		if(final > 0.0)
			final -= 0.1;

		std::cout << "> keyboardEvent: " << final << std::endl;
	}

	if(key == '3' && state)
	{
		filtro = GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtro);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtro);
	}

	if(key == '4' && state)
	{
		filtro = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtro);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtro);
	}

	if(key == 27 && state)
	{
		engine->getGameWindow()->destroy();
	}
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
	renderingSystem->setViewport(0, 0, width, height);
	renderingSystem->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-cameraX, -cameraY, -cameraZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
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

	renderingSystem->setViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	renderingSystem->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	img = new DIB();
	img->loadFile("azulejo.bmp", true);

	GLuint id;
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		img->getWidth(),
		img->getHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		img->getColorIndex());

	// Ajusta os filtros iniciais para a textura
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtro);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtro);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void GameEventHandler::afterMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}