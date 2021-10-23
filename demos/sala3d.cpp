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
double cameraY = 150.0;
double cameraZ = 100.0;

float ambientLight = 0.1f;
float diffuseLight = 0.3f;
float specularLight = 0.0f;

float posLight[] = {
	0.0, 200.0,  250.0, 1.0,
	0.0, 200.0,    0.0, 1.0,
	0.0, 200.0, -250.0, 1.0,
	1.0,   0.2,    0.0, 0.0,
	0.0,   0.0,    0.0, 0.0
};

float materialAmbient   = 0.0f;
float materialSpecular  = 0.0f;
float materialDiffuse   = 0.0f;
float materialShininess = 0.0f;
float materialEmission  = 0.0f;

int render_type = 1;

typedef struct
{
	float x,z;		// centro da mesa
	float y;		// altura para os objetos
} MESA;

MESA limites[37];

int init_x, init_y;
double init_rotateX, init_rotateY;
double init_cameraX, init_cameraY, init_cameraZ;

int buttonState = -1;

KEngine* engine;
KERenderingSystem* renderingSystem;

KEModel* models[TOTAL_OBJ];
// KEObject obj[TOTAL_OBJ];

typedef struct {
	float x, y, z;
};

KELight *light1;
KELight *light2;
KELight *light3;
KELight *light4;
KELight *light5;

bool lightStatus[5] = {1, 1, 1, 1, 1};

void drawObjects();
void DefineLimitesMesas();

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
	renderingSystem->setProjectionZNear(0.1);
	renderingSystem->setProjectionZFar(1000.0);
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

	for(int i = 0; i < TOTAL_OBJ; i++)
	{
		delete models[i];
	}

	delete renderingSystem;
	delete engine;
	return 1;
}

void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(rotateX, 1,0,0);
	glRotatef(rotateY, 0,1,0);
	glTranslatef(-cameraX, -cameraY, -cameraZ);

	glColor3f(1.0f, 1.0f, 1.0f);

	setLight(*light1, lightStatus[0], GL_LIGHT0);
	setLight(*light2, lightStatus[1], GL_LIGHT1);
	setLight(*light3, lightStatus[2], GL_LIGHT2);
	setLight(*light4, lightStatus[3], GL_LIGHT3);
	setLight(*light5, lightStatus[4], GL_LIGHT4);

	drawObjects();
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

	std::cout << "> keyboard event - key: " << key << " - state: " << state << std::endl;

	if(key == 84 && state )
	{
		render_type = (render_type ? 0 : 1);
	}

	if(key == '1' && state)
	{
		lightStatus[0] = (lightStatus[0] ? 0 : 1);
		setLight(*light1, lightStatus[0], GL_LIGHT0);
	}

	if(key == '2' && state)
	{
		lightStatus[1] = (lightStatus[1] ? 0 : 1);
		setLight(*light2, lightStatus[1], GL_LIGHT1);
	}

	if(key == '3' && state)
	{
		lightStatus[2] = (lightStatus[2] ? 0 : 1);
		setLight(*light3, lightStatus[2], GL_LIGHT2);
	}

	if(key == '4' && state)
	{
		lightStatus[3] = (lightStatus[3] ? 0 : 1);
		setLight(*light4, lightStatus[3], GL_LIGHT3);
	}

	if(key == '5' && state)
	{
		lightStatus[4] = (lightStatus[4] ? 0 : 1);
		setLight(*light5, lightStatus[4], GL_LIGHT4);
	}

	if(key == 87 && state)
	{
		float sina = 5 * sin( rotateY * 3.1415 / 180.0);
		float cosa = 5 * cos( rotateY * 3.1415 / 180.0);

		cameraX += 0.2 * sina;
		cameraZ -= 0.2 * cosa;
	}

	if(key == 83 && state)
	{
		float sina = 5 * sin( rotateY * 3.1415 / 180.0);
		float cosa = 5 * cos( rotateY * 3.1415 / 180.0);

		cameraX -= 0.2 * sina;
		cameraZ += 0.2 * cosa;
	}

	// if(key == '1' && state)
	// {
	// 	double fovy = renderingSystem->getProjectionFOVY();
	// 	renderingSystem->setProjectionFOVY(fovy + 1.0);
	// 	renderingSystem->setProjection();

	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();

	// 	glTranslatef(-cameraX, -cameraY, -cameraZ);
	// 	glRotatef(rotateX, 1, 0, 0);
	// 	glRotatef(rotateY, 0, 1, 0);
	// }

	// if(key == '2' && state)
	// {
	// 	double fovy = renderingSystem->getProjectionFOVY();
	// 	renderingSystem->setProjectionFOVY(fovy - 1.0);
	// 	renderingSystem->setProjection();

	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();

	// 	glTranslatef(-cameraX, -cameraY, -cameraZ);
	// 	glRotatef(rotateX, 1, 0, 0);
	// 	glRotatef(rotateY, 0, 1, 0);
	// }

	// if(key == '3' && state)
	// {
	// 	model_index++;

	// 	if(model_index > 5)
	// 		model_index = 0;

	// 	obj = model[model_index]; 
	// }

	// if(key == '4' && state)
	// {
	// 	cameraZ += 100.0;
	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();

	// 	glTranslatef(-cameraX, -cameraY, -cameraZ);
	// 	glRotatef(rotateX, 1, 0, 0);
	// 	glRotatef(rotateY, 0, 1, 0);
	// }

	// if(key == '5' && state)
	// {
	// 	cameraZ -= 100.0;
	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();

	// 	glTranslatef(-cameraX, -cameraY, -cameraZ);
	// 	glRotatef(rotateX, 1, 0, 0);
	// 	glRotatef(rotateY, 0, 1, 0);
	// }

	// if(key == '9' && state)
	// {
	// 	ambientLight += 0.1;

	// 	if(ambientLight > 1.0)
	// 		ambientLight = 0.0;

	// 	std::cout << "> ambientLight: " << ambientLight << std::endl;
	// 	light->setAmbient(ambientLight, ambientLight, ambientLight, 1.0);
	// 	setLight(*light, 1);
	// }

	// if(key == '0' && state)
	// {
	// 	diffuseLight += 0.1;

	// 	if(diffuseLight > 1.0)
	// 		diffuseLight = 0.0;

	// 	std::cout << "> diffuseLight: " << diffuseLight << std::endl;
	// 	light->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0);
	// 	setLight(*light, 1);
	// }

	// if(key == '8' && state)
	// {
	// 	specularLight += 0.1;

	// 	if(specularLight > 1.0)
	// 		specularLight = 0.0;

	// 	std::cout << "> specularLight: " << specularLight << std::endl;
	// 	light->setSpecular(specularLight, specularLight, specularLight, 1.0);
	// 	setLight(*light, 1);
	// }

	// if(key == 81 && state)
	// {
	// 	materialAmbient += 0.1;

	// 	if(materialAmbient > 1.0)
	// 		materialAmbient = 0.0;

	// 	std::cout << "> materialAmbient: " << materialAmbient << std::endl;
	// 	float mAmbient[4] = {0.0f, materialAmbient, 0.0f, 1.0f};
	// 	glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbient);
	// }

	// if(key == 87 && state)
	// {
	// 	materialSpecular += 0.1;

	// 	if(materialSpecular > 1.0)
	// 		materialSpecular = 0.0;

	// 	std::cout << "> materialSpecular: " << materialSpecular << std::endl;
	// 	float mSpecular[4] = {materialSpecular, materialSpecular, materialSpecular, 1.0f};
	// 	glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecular);
	// }

	// if(key == 69 && state)
	// {
	// 	materialDiffuse += 0.1;

	// 	if(materialDiffuse > 1.0)
	// 		materialDiffuse = 0.0;

	// 	std::cout << "> materialDiffuse: " << materialDiffuse << std::endl;
	// 	float mDiffuse[4] = {materialDiffuse, materialDiffuse, materialDiffuse, 1.0f};
	// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	// }

	// if(key == 82 && state)
	// {
	// 	materialShininess += 0.1;

	// 	if(materialShininess > 1.0)
	// 		materialShininess = 0.0;

	// 	std::cout << "> materialShininess: " << materialShininess << std::endl;
	// 	float mDiffuse[4] = {materialShininess, materialShininess, materialShininess, 1.0f};
	// 	glMaterialfv(GL_FRONT, GL_SHININESS, mDiffuse);
	// }

	// if(key == 84 && state)
	// {
	// 	materialEmission += 0.1;

	// 	if(materialEmission > 1.0)
	// 		materialEmission = 0.0;

	// 	std::cout << "> materialEmission: " << materialEmission << std::endl;
	// 	float mDiffuse[4] = {materialEmission, materialEmission, materialEmission, 1.0f};
	// 	glMaterialfv(GL_FRONT, GL_EMISSION, mDiffuse);
	// }

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	renderingSystem->setLightModelAmbient(ambientLight, ambientLight, ambientLight, 1.0);
	renderingSystem->setLightModel(1);
	
	light1 = new KELight();
	light1->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	light1->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	light1->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	light1->setPosition(posLight[0], posLight[1], posLight[2], posLight[3]);
	setLight(*light1, lightStatus[0], GL_LIGHT0);

	light2 = new KELight();
	light2->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	light2->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	light2->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	light2->setPosition(posLight[4], posLight[5], posLight[6], posLight[7]);
	setLight(*light2, lightStatus[1], GL_LIGHT1);

	light3 = new KELight();
	light3->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	light3->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	light3->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	light3->setPosition(posLight[8], posLight[9], posLight[10], posLight[11]);
	setLight(*light3, lightStatus[2], GL_LIGHT2);

	float dl[] = {0.4f, 0.2f, 0.0f, 1.0f};
	light4 = new KELight();
	light4->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);

	light4->setDiffuse(dl[0], dl[1], dl[2], 1.0f);
	light4->setSpecular(specularLight, specularLight, specularLight, 1.0f);
	light4->setPosition(posLight[12], posLight[13], posLight[14], posLight[15]);
	setLight(*light4, lightStatus[3], GL_LIGHT3);

	light5 = new KELight();
	light5->setAmbient(ambientLight, ambientLight, ambientLight, 1.0f);
	light5->setDiffuse(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	light5->setPosition(posLight[16], posLight[17], posLight[18], posLight[19]);
	setLight(*light5, lightStatus[4], GL_LIGHT4);

	float mAmbient[4] = {0.2, 0.2, 0.2, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmbient);

	float mDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDiffuse);

	for(int i = 0; i < TOTAL_OBJ; i++)
	{
		models[i] = new KEModel();
	}

	models[0]->loadfile("parede.obj");
	models[1]->loadfile("mesagrande.obj");
	models[2]->loadfile("mesapeq.obj");
	models[3]->loadfile("cadeira2.obj");
	models[4]->loadfile("quadronegro.obj");
	models[5]->loadfile("porta.obj");
	models[6]->loadfile("janela.obj");
	models[7]->loadfile("lampada.obj");
	models[8]->loadfile("lapis.obj");
	models[9]->loadfile("livro.obj");
	models[10]->loadfile("papel1.obj");
	models[11]->loadfile("papel2.obj");
	models[12]->loadfile("papel3.obj");
	models[13]->loadfile("cuiabomba.obj");
	models[14]->loadfile("borracha.obj");

	renderingSystem->setViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	renderingSystem->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-cameraX, -cameraY, -cameraZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);

	DefineLimitesMesas();
}

void GameEventHandler::afterMainLoopEvent()
{
}

void GameEventHandler::createWindowEvent()
{
}

// ----------------------------------------------------------------------------
void drawObjects()
{
	glPushMatrix();
	glTranslatef(-301, 100, 300);
	glRotatef(90, 0, 1, 0);
	drawModel(*models[5], render_type);
	glPopMatrix();

	// Seta cor da parede
	glColor3ub(196,210,184);
	// Parede dos fundos
	glPushMatrix();
	glTranslatef(0, 150, -400);
	glScalef(6, 3, 1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 150, 400);
	glRotatef(180, 0, 1, 0);
	glScalef(6, 3, 1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-300, 150, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(8, 3, 1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,37.5,0);
	glRotatef(-90,0,1,0);
	glScalef(8,0.75,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,300-37.5,0);
	glRotatef(-90,0,1,0);
	glScalef(8,0.75,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,150,-375);
	glRotatef(-90,0,1,0);
	glScalef(0.5,1.5,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,150,-125);
	glRotatef(-90,0,1,0);
	glScalef(0.5,1.5,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,150,125);
	glRotatef(-90,0,1,0);
	glScalef(0.5,1.5,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300,150,375);
	glRotatef(-90,0,1,0);
	glScalef(0.5,1.5,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300, 150, -250);
	glRotatef(-90, 0, 1, 0);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300, 150, 0);
	glRotatef(-90, 0, 1, 0);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(300, 150, 250);
	glRotatef(-90, 0, 1, 0);
	drawModel(*models[0], render_type);
	glPopMatrix();

	for(int x = -150; x <= 300; x += 300)
	{
		int cont = 0;
		// 3 filas de lâmpadas
		for(int z = -250; z <= 300; z += 250, cont++)
		{
			// float emissao = 0.0;
			// Seta o coeficiente de emissão para
			// as luzes que estiverem ativas
			// if(luzes[2- c ont]) emissao = 1.0;
			// mat_luz->ke[0] = mat_luz->ke[1] = mat_luz->ke[2] = emissao;
			glPushMatrix();
			glTranslatef(x, 297, z);
			glRotatef(90, 1, 0, 0);
			glRotatef(90, 0, 0, 1);
			drawModel(*models[7], render_type);
			glPopMatrix();
		}
	}

	glColor3ub(145,105,54);
	glPushMatrix();
	glTranslatef(0,0,0);
	glRotatef(-90,1,0,0);
	glScalef(6,8,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glColor3ub(212,199,183);
	glPushMatrix();
	glTranslatef(0,300,0);
	glRotatef(90,1,0,0);
	glScalef(6,8,1);
	drawModel(*models[0], render_type);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(limites[36].x,limites[36].y-12,limites[36].z);
	drawModel(*models[1], render_type);

	glTranslatef(0,-15,30);
	glRotatef(180,0,1,0);
	drawModel(*models[3], render_type);

	glPopMatrix();

	for(int i = 0; i < 36; i++)
	{
		glPushMatrix();

		glTranslatef(limites[i].x,limites[i].y-10,limites[i].z);

		drawModel(*models[2], render_type);

		glTranslatef(0,-10,-30);
		drawModel(*models[3], render_type);
		glPopMatrix();
	}

	int mesaIndex[] = {0, 9, 9, 9, 9, 36, 36, 36, 36, 36, 15, 15, 15, 20, 36, 8};
	// int tipoIndex[] = {0, 0, 1, 3, 4,  0,  1,  1,  0,  5,  0,  2,  3,  1,  1, 6};
	int tipoIndex[] = {8, 8, 9, 11, 12,  8,  9,  9,  8,  13,  8,  10,  11,  9,  9, 14};

	float t[] = {
		-9.000000, 0.000000, -4.000000, //0
		-37.000000, 0.000000, 6.000000,// 3
		-18.000000, 0.000000, 5.000000, // 6
		-26.000000, 0.000000, 6.000000, // 9
		-18.000000, 0.010000, 5.000000,
		31.000000, 0.000000, 1.000000,
		-46.000000, 0.000000, 12.000000,
		-46.000000, 3.200001, 11.000000,
		35.000000, 0.000000, 9.000000,
		16.000000, 7.000000, 19.000000,
		-22.000000, 0.000000, 0.000000,
		2.000000, 0.000000, -2.000000,
		8.000000, 0.200000, -2.000000,
		11.000000, 0.000000, -2.000000,
		-46.000000, 6.400002, 13.000000,
		19.399998, 0.600000, 11.000000
	};

	float r[] = {
		153.000000, 0.000000, 1.000000, 0.000000,
		405.000000, 0.000000, 1.000000, 0.000000,
		42.000000, 0.000000, 1.000000, 0.000000,
		-70.000000, 0.000000, 1.000000, 0.000000,
		65.000000, 0.000000, 1.000000, 0.000000,
		0.000000, 0.000000, 1.000000, 0.000000,
		187.000000, 0.000000, 1.000000, 0.000000,
		177.000000, 0.000000, 1.000000, 0.000000,
		0.000000, 0.000000, 1.000000, 0.000000,
		-130.000000, 0.000000, 1.000000, 0.000000,
		-206.000000, 0.000000, 1.000000, 0.000000,
		-13.000000, 0.000000, 1.000000, 0.000000,
		10.000000, 0.000000, 1.000000, 0.000000,
		-17.000000, 0.000000, 1.000000, 0.000000,
		185.000000, 0.000000, 1.000000, 0.000000,
		139.000000, 0.000000, 1.000000, 0.000000
	};

	for(int tr = 0, rot = 0, i = 0; i < 16; i++, tr += 3, rot += 4)
	{
		glPushMatrix();

		MESA mesa = limites[mesaIndex[i]];

		glTranslatef(mesa.x,mesa.y,mesa.z);
		// glTranslatef(objetos[i]->transl[0],objetos[i]->transl[1],objetos[i]->transl[2]);
		glTranslatef(t[tr], t[tr * 3 + 1], t[tr + 2]);

		// glRotatef(objetos[i]->rot[0],objetos[i]->rot[1],objetos[i]->rot[2],objetos[i]->rot[3]);
		glRotatef(r[rot], r[rot + 1], r[rot + 2], r[rot + 3]);
		drawModel(*models[tipoIndex[i]], render_type);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0,160,398);
	glRotatef(180,0,1,0);
	drawModel(*models[4], render_type);
	glPopMatrix();

	// IF MODO DESENHO
	// glDisable(GL_LIGHTING);
	glBegin(GL_QUAD_STRIP);
	glColor3ub(239,235,55);
	glVertex3f(310,70,-500);
	glVertex3f(310,70, 500);
	glColor3ub(255,136,0);
	glVertex3f(310,85,-500);
	glVertex3f(310,85,500);
	glColor3ub(170,11,61);
	glVertex3f(310,100,-500);
	glVertex3f(310,100,500);
	glColor3ub(28,17,145);
	glVertex3f(310,150,-500);
	glVertex3f(310,150,500);
	glColor3ub(0,0,0);
	glVertex3f(310,250,-500);
	glVertex3f(310,250,500);
	glEnd();
	// glEnable(GL_LIGHTING);
}

void DefineLimitesMesas()
{
	int l, c, pos;
	float x, z;

	z = -320;
	pos = 0;

	for(l = 0; l <= 5; l++)
	{
		x = -220;

		for(c = 0; c < 3; c++)
		{
			limites[pos].x = x;
			limites[pos].z = z;	
			limites[pos].y = 57;
			pos++;
			
			x = x + 61;
			limites[pos].x = x;
			limites[pos].z = z;	
			limites[pos].y = 57;
			pos++;
		
			x = x + 120;
		}

		z = z + 95;
	}

	limites[pos].x = 200;
	limites[pos].z = 300;	
	limites[pos].y = 70;
}