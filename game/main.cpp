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

#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>

#define WINDOW_WIDTH  1016
#define WINDOW_HEIGHT 800

/*
typedef struct {
	float x, y, z;
} VERTEX;

typedef struct {
	int total_of_vertices;
	int vertex_index[4];
} FACE;

typedef struct {
	VERTEX *vertices;
	FACE *faces;
	int total_of_faces;
} OBJ;

typedef struct {
	VERTEX *vertices;
	COLOR *colors;
	unsigned int *indices;
	
	unsigned int total_indices;
	unsigned int total_vertex;
} MODEL;
*/

// Definição dos vértices
GEOMETRIC_VERTEX vertices[] = {
	{ -1, 0, -1 },	// 0 canto inf esquerdo tras.
	{  1, 0, -1 },	// 1 canfo inf direito  tras.
	{  1, 0,  1 },	// 2 canto inf direito  diant.
	{ -1, 0,  1 },  // 3 canto inf esquerdo diant.
	{  0, 2,  0 },  // 4 topo
};

// Definição das faces
FACE faces[] = {
	{ 4, { 0,1,2, 3 }},	// base
	{ 3, { 0,1,4,-1 }},	// lado traseiro
	{ 3, { 0,3,4,-1 }},	// lado esquerdo
	{ 3, { 1,2,4,-1 }},	// lado direito
	{ 3, { 3,2,4,-1 }}	// lado dianteiro
};

OBJ piramide = {vertices, faces, 5};

double rotateX = 0.0;
double rotateY = 0.0;
double rotateZ = 0.0;
double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 5.0;

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
KERenderingSystem* renderingSystem;
// DIB* image;
OBJReader* obj; 

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
	renderingSystem->setProjectionZNear(0.1);
	renderingSystem->setProjectionZFar(1200.0);
	renderingSystem->setProjectionFOVY(60.0);
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

	// image->release();
	// delete image;

	delete obj;

	delete renderingSystem;
	delete engine;
	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT);

	// glColor3f(0.0f, 0.0f, 0.0f);

	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();

	// glTranslatef(-cameraX, -cameraY, -cameraZ);
	// glRotatef(rotateX, 1, 0, 0);
	// glRotatef(rotateY, 0, 1, 0);

	// drawOBJ(&piramide);

	// glRasterPos2i(0, 0);

	// glDrawPixels(
	// 	image->getWidth(),
	// 	image->getHeight(),
	// 	GL_RGB,
	// 	GL_UNSIGNED_BYTE,
	// 	image->getColorIndex());
}

int init_x, init_y;
double init_rotateX, init_rotateY;
double init_cameraX, init_cameraY, init_cameraZ;

int buttonState = -1;
#define SENS_ROTATE 5.0
#define SENSE_CAMERA 30.0

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
	// std::cout << "@mouseMotionEvent | x: " << x << " | y: " << y << std::endl;

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
		double fovy = renderingSystem->getProjectionFOVY();
		renderingSystem->setProjectionFOVY(fovy + 1.0);
		renderingSystem->setProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

	if(key == '2' && state)
	{
		double fovy = renderingSystem->getProjectionFOVY();
		renderingSystem->setProjectionFOVY(fovy - 1.0);
		renderingSystem->setProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

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

	// image = new DIB();
	// image->loadFile("24bpp_test.bmp", true);

	obj = new OBJReader();

	for(int i = 0; i < 1; i++)
	{
		obj->loadfile("mech.obj");
	}

	renderingSystem->setViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	renderingSystem->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-cameraX, -cameraY, -cameraZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
}

void GameEventHandler::afterMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}