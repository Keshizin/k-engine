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

#include <GL/gl.h>
#include <gl/glu.h>
#include <iostream>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT  800
#define SENS_ROTATE   5.0
#define SENSE_CAMERA  10.0

double rotateX = 0.0;
double rotateY = 0.0;
double rotateZ = 0.0;
double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 5.0;

float ambientLight = 0.1f;
float diffuseLight = 0.3f;
float specularLight = 0.0f;

float posLight[] = {
	0.0, 200.0,  250.0, 1.0,
};

float materialAmbient   = 0.0f;
float materialSpecular  = 0.0f;
float materialDiffuse   = 0.0f;
float materialShininess = 0.0f;
float materialEmission  = 0.0f;

int render_type = 1;

int init_x, init_y;
double init_rotateX, init_rotateY;
double init_cameraX, init_cameraY, init_cameraZ;

int buttonState = -1;

KEngine* engine;
KERenderingSystem* renderingSystem;
KEModel* models;
KELight *light1;

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
	renderingSystem->setRenderingContext(K_CONTEXT_3D_PERSPECTIVE);
	renderingSystem->setRenderingWindow(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
	renderingSystem->setProjectionZNear(1.0);
	renderingSystem->setProjectionZFar(250.0);
	renderingSystem->setProjectionFOVY(60.0);
	renderingSystem->setWindowAspectCorrectionState(true);

	// Setting up the window
	engine->getGameWindow()->setWindow(1000, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "K-ENGINE DEMO", K_WINDOW_COMPLETE);

	// Creating the window
	engine->getGameWindow()->create();

	renderingSystem->initialize();

	// Showing the window
	engine->getGameWindow()->show(nCmdShow);

	renderingSystem->setVSync(0);
	engine->setFrameRate(0);

	// Starting the game loop
	engine->startMainLoop();

	delete models;
	delete renderingSystem;
	delete engine;
	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// glRotatef(rotateX, 1,0,0);
	// glRotatef(rotateY, 0,1,0);
	// glTranslatef(-cameraX, -cameraY, -cameraZ);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	// drawModel(*models, render_type);
	drawModel2(*models, render_type);
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

	if(key == 87 && state)
	{
		float sina = 10 * sin( rotateY * 3.1415 / 180.0);
		float cosa = 10 * cos( rotateY * 3.1415 / 180.0);

		cameraX += 0.2 * sina;
		cameraZ -= 0.2 * cosa;
	}

	if(key == 83 && state)
	{
		float sina = 10 * sin( rotateY * 3.1415 / 180.0);
		float cosa = 10 * cos( rotateY * 3.1415 / 180.0);

		cameraX -= 0.2 * sina;
		cameraZ += 0.2 * cosa;
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
	K_UNREFERENCED_PARAMETER(width);
	K_UNREFERENCED_PARAMETER(height);

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
	// GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// glShadeModel(GL_FLAT);

	// renderingSystem->setLightModelAmbient(ambientLight, ambientLight, ambientLight, 1.0);
	// renderingSystem->setLightModel(1);
	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	// light1 = new KELight();
	// light1->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	// light1->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	// light1->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	// light1->setPosition(posLight[0], posLight[1], posLight[2], posLight[3]);
	// setLight(*light1, lightStatus[0], GL_LIGHT0);

	// float mAmbient[4] = {0.2, 0.2, 0.2, 1.0f};
	// glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmbient);

	// float mDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDiffuse);

	// glEnable(GL_COLOR_MATERIAL);
	// glColorMaterial(GL_FRONT, GL_DIFFUSE);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	models = new KEModel();
	models->loadfile("obj/", "cubo.obj");

	renderingSystem->setViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	renderingSystem->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-cameraX, -cameraY, -cameraZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);

	setVertexArray(*models);
}

void GameEventHandler::afterMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}