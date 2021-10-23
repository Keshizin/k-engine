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

#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>

#define WINDOW_WIDTH  1016
#define WINDOW_HEIGHT 800

double rotateX = 0.0;
double rotateY = 0.0;
double rotateZ = 0.0;
double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 100.0;

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
DIB* image;
KEModel* model[6];
KEModel* obj = 0;
KEModel* floorOBJ;
int model_index = 0;
KELight *light = 0;

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
	renderingSystem->setProjectionZFar(3200.0);
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

	image->release();
	delete image;

	delete light;

	delete model[0];
	delete model[1];
	delete model[2];
	delete model[3];
	delete model[4];
	delete model[5];
	delete floorOBJ;

	delete renderingSystem;
	delete engine;
	return 1;
}

float ambientLight = 0.0f;
float diffuseLight = 0.0f;
float specularLight = 0.0f;

float materialAmbient = 0.0f;
float materialSpecular = 0.0f;
float materialDiffuse = 0.0f;
float materialShininess = 0.0f;
float materialEmission = 0.0f;

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-cameraX, -cameraY, -cameraZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex4fv(light->position);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);

	// if(floorOBJ)
	// 	drawModel(*floorOBJ);

	if(obj)
		drawModel(*obj);
}

int init_x, init_y;
double init_rotateX, init_rotateY;
double init_cameraX, init_cameraY, init_cameraZ;

int buttonState = -1;
#define SENS_ROTATE 5.0
#define SENSE_CAMERA 10.0

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
	std::cout << "debug: " << key << std::endl;

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

	if(key == '3' && state)
	{
		model_index++;

		if(model_index > 5)
			model_index = 0;

		obj = model[model_index]; 
	}

	if(key == '4' && state)
	{
		cameraZ += 100.0;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

	if(key == '5' && state)
	{
		cameraZ -= 100.0;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-cameraX, -cameraY, -cameraZ);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
	}

	if(key == '9' && state)
	{
		ambientLight += 0.1;

		if(ambientLight > 1.0)
			ambientLight = 0.0;

		std::cout << "> ambientLight: " << ambientLight << std::endl;
		light->setAmbient(ambientLight, ambientLight, ambientLight, 1.0);
		setLight(*light, 1);
	}

	if(key == '0' && state)
	{
		diffuseLight += 0.1;

		if(diffuseLight > 1.0)
			diffuseLight = 0.0;

		std::cout << "> diffuseLight: " << diffuseLight << std::endl;
		light->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0);
		setLight(*light, 1);
	}

	if(key == '8' && state)
	{
		specularLight += 0.1;

		if(specularLight > 1.0)
			specularLight = 0.0;

		std::cout << "> specularLight: " << specularLight << std::endl;
		light->setSpecular(specularLight, specularLight, specularLight, 1.0);
		setLight(*light, 1);
	}

	if(key == 81 && state)
	{
		materialAmbient += 0.1;

		if(materialAmbient > 1.0)
			materialAmbient = 0.0;

		std::cout << "> materialAmbient: " << materialAmbient << std::endl;
		float mAmbient[4] = {0.0f, materialAmbient, 0.0f, 1.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbient);
	}

	if(key == 87 && state)
	{
		materialSpecular += 0.1;

		if(materialSpecular > 1.0)
			materialSpecular = 0.0;

		std::cout << "> materialSpecular: " << materialSpecular << std::endl;
		float mSpecular[4] = {materialSpecular, materialSpecular, materialSpecular, 1.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecular);
	}

	if(key == 69 && state)
	{
		materialDiffuse += 0.1;

		if(materialDiffuse > 1.0)
			materialDiffuse = 0.0;

		std::cout << "> materialDiffuse: " << materialDiffuse << std::endl;
		float mDiffuse[4] = {materialDiffuse, materialDiffuse, materialDiffuse, 1.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	}

	if(key == 82 && state)
	{
		materialShininess += 0.1;

		if(materialShininess > 1.0)
			materialShininess = 0.0;

		std::cout << "> materialShininess: " << materialShininess << std::endl;
		float mDiffuse[4] = {materialShininess, materialShininess, materialShininess, 1.0f};
		glMaterialfv(GL_FRONT, GL_SHININESS, mDiffuse);
	}

	if(key == 84 && state)
	{
		materialEmission += 0.1;

		if(materialEmission > 1.0)
			materialEmission = 0.0;

		std::cout << "> materialEmission: " << materialEmission << std::endl;
		float mDiffuse[4] = {materialEmission, materialEmission, materialEmission, 1.0f};
		glMaterialfv(GL_FRONT, GL_EMISSION, mDiffuse);
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

	glEnable(GL_DEPTH_TEST);

	renderingSystem->setLightModelAmbient(ambientLight, ambientLight, ambientLight, 1.0);
	renderingSystem->setLightModel(1);
	
	light = new KELight();
	light->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	light->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	light->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	light->setPosition(0.0f, 5.0f, 0.0f, 1.0f);
	setLight(*light, 1);

	// float mAmbient[4] = {materialAmbient, materialAmbient, materialAmbient, 1.0f};
	// glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbient);
	// float mSpecular[4] = {materialSpecular, materialSpecular, materialSpecular, 1.0f};
	// glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecular);
	// float mDiffuse[4] = {materialDiffuse, materialDiffuse, materialDiffuse, 1.0f};
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	// float mShininess[4] = {materialShininess, materialShininess, materialShininess, 1.0f};
	// glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);
	// float mEmission[4] = {materialEmission, materialEmission, materialEmission, 1.0f};
	// glMaterialfv(GL_FRONT, GL_EMISSION, mEmission);

	// image = new DIB();
	// image->loadFile("24bpp_test.bmp", true);

	model[0] = new KEModel();
	model[1] = new KEModel();
	model[2] = new KEModel();
	model[3] = new KEModel();
	model[4] = new KEModel();
	// model[5] = new KEModel();

	// model[0]->loadfile("cadeira.obj");
	// model[1]->loadfile("cubo.obj");
	// model[2]->loadfile("deer.obj");
	// model[3]->loadfile("mech.obj");
	// model[4]->loadfile("mesagrande.obj");
	model[1]->loadfile("esfera.obj");

	floorOBJ = new KEModel();
	floorOBJ->loadfile("floor.obj");

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