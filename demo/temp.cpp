/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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
#include <keaux.h>
#include <kerenderingsystem.h>
#include <keentity.h>
#include <kecamera.h>
#include <ketimer.h>

#include <list>

#define MONITOR_WIDTH	2560
#define MONITOR_HEIGHT	1080
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	800
#define TOTAL_ENTITY	1


// ----------------------------------------------------------------------------
//  kengine::sprite class (testing)
// 
// (!) Classe é candidata para fazer parte de core
// ----------------------------------------------------------------------------
class sprite : public kengine::entity
{
public:
	sprite(kengine::atlas* a)
		:
		atlas{ a },
		visible{ true },
		anim{ kengine::ANIMATION_TYPE::CONTINUOUS, 0, 10, 50 }
	{}

	~sprite() {}

	sprite(const sprite& copy) = delete; // copy constructor
	sprite(sprite&& move) noexcept = delete; // move constructor
	sprite& operator=(const sprite& copy) = delete; // copy assignment

	void copyFrame(float* uv)
	{
		atlas->copyFrame(anim.getCurrentFrame(), uv);
	}

	void start()
	{
		anim.start();
	}

	void update(double frameTime)
	{
		K_UNREFERENCED_PARAMETER(frameTime);
		anim.update();
	}

	kengine::vec3<float> translate;
	kengine::vec3<float> rotate;
	kengine::vec3<float> scale;

private:
	kengine::atlas* atlas;
	bool visible;
	kengine::animation2D anim;
};


// ----------------------------------------------------------------------------
//  GameObejct class (testing)
// 
// (!) classe é candidata para fazer parte de core
// ----------------------------------------------------------------------------
class GameObject : public kengine::entity
{
public:
	GameObject() : translate{}, rotate{}, scale{} {}
	~GameObject() {}

	GameObject(const GameObject& copy) = delete; // copy constructor
	GameObject(GameObject&& move) noexcept = delete; // move constructor
	GameObject& operator=(const GameObject& copy) = delete; // copy assignment

	void update(double frameTime)
	{
		K_UNREFERENCED_PARAMETER(frameTime);
	}

	kengine::vec3<float> translate;
	kengine::vec3<float> rotate;
	kengine::vec3<float> scale;
};


// ----------------------------------------------------------------------------
//  Game Engine Core Events
// ----------------------------------------------------------------------------
class GameEventHandler : public kengine::eventhandler
{
public:
	void createWindowEvent();
	void beforeMainLoopEvent();
	void afterMainLoopEvent();
	void finishBeforeEvent();
	void finishAfterEvent();
	void frameEvent(double frameTime);
	void resumeEvent();
	void pauseEvent();
	void mouseEvent(int button, int state, int x, int y);
	void mouseMotionEvent(int x, int y);
	void keyboardEvent(unsigned long long key, int state);
	void keyboardSpecialEvent(unsigned long long key, int state);
	void resizeWindowEvent(int width, int height);
	void moveWindowEvent(int x, int y);
};


// k-engine core
kengine::core* myCore;

// rendering system
kengine::renderingsystem* myRenderingSystem;

// transform shader program
kengine::TransformProgram* myShader;

// instanced model node
kengine::instanced_uv_mesh_node* myBatch;

// point mesh batch
kengine::primitive_mesh_batch* primPoint;
kengine::primitive_mesh_batch* primLine;

GLfloat* modelViewData;
GLfloat* uvData;
GLfloat* pointsData;
GLfloat* linesData;

std::list<kengine::entity*> entityList;

// entity list and model view buffer object data
sprite* mySprite;

// camera object
kengine::camera camera;

// texture object
kengine::texture* myTexture;
kengine::atlas* myAtlas;

// ----------------------------------------------------------------------------
//  Point Entry Execution
// ----------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);
	K_UNREFERENCED_PARAMETER(nCmdShow);

	GameEventHandler myEvents;
	myCore = new kengine::core(&myEvents);

	myCore->getGameWindow()->create(
		(MONITOR_WIDTH / 2) - (WINDOW_WIDTH / 2),
		(MONITOR_HEIGHT / 2) - (WINDOW_WIDTH / 2),
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"K-ENGINE DEMO",
		K_WINDOW_COMPLETE);

	myRenderingSystem = new kengine::renderingsystem(myCore->getWin32api(), kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D);
	myRenderingSystem->startup();
	myRenderingSystem->setVSync(0);

	myCore->getGameWindow()->show(nCmdShow);
	myCore->setFrameRate(0);
	myCore->startMainLoop();

	delete myTexture;
	delete[] modelViewData;
	delete[] uvData;

	for (auto e : entityList)
	{
		delete e;
	}

	//delete mySprite;
	delete primPoint;
	delete primLine;
	delete myBatch;
	delete[] myShader;
	delete myRenderingSystem;
	delete myCore;

	return 1;
}


// ----------------------------------------------------------------------------
//  GameEventHandler method's definition
// ----------------------------------------------------------------------------
void GameEventHandler::createWindowEvent()
{
}


void GameEventHandler::beforeMainLoopEvent()
{
	// setting up rendering system
	myRenderingSystem->printInfo();
	myRenderingSystem->setPolygonMode(K_RENDERING_MODE_FILL);
	myRenderingSystem->setBlendingTest(1);


	// setting up shaders
	myShader = new kengine::TransformProgram[2];

	kengine::ShaderInfo shaders[] = {
			{GL_VERTEX_SHADER, "../../../../shaders/main_vert.vert"},
			{GL_FRAGMENT_SHADER, "../../../../shaders/main_frag.frag"},
			{GL_NONE, ""}
	};

	myShader[0].loadShaders(shaders);

	kengine::ShaderInfo shaders2[] = {
		{GL_VERTEX_SHADER, "../../../../shaders/prim_vert.vert"},
		{GL_FRAGMENT_SHADER, "../../../../shaders/prim_frag.frag"},
		{GL_NONE, ""}
	};

	myShader[1].loadShaders(shaders2);


	// setting up mesh
	kengine::mesh m = kengine::quad(0.5f);

	// allocating entityies data to memory
	myBatch = new kengine::instanced_uv_mesh_node(TOTAL_ENTITY, m);
	modelViewData = new GLfloat[TOTAL_ENTITY * 16];
	uvData = new GLfloat[TOTAL_ENTITY * 8];

	float blueColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	primPoint = new kengine::primitive_mesh_batch(1 * 4, kengine::PRIMITIVE_TYPE::PRIMITIVE_POINT, blueColor);
	pointsData = new GLfloat[1 * 4];
	pointsData[0] = 0.5f;
	pointsData[1] = 0.0f;
	pointsData[2] = 0.0f;
	pointsData[3] = 1.0f;

	float redColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	primLine = new kengine::primitive_mesh_batch(1 * 8, kengine::PRIMITIVE_TYPE::PRIMITIVE_LINE, redColor);
	linesData = new GLfloat[1 * 8];
	linesData[0] = -2.0f;
	linesData[1] = 0.0f;
	linesData[2] = 0.0f;
	linesData[3] = 1.0f;
	linesData[4] = 2.0f;
	linesData[5] = 0.0f;
	linesData[6] = 0.0f;
	linesData[7] = 1.0f;


	// setting up textures
	myAtlas = new kengine::atlas("../../../../assets/moeda.png.KRAW", 10, 0);


	// setting up game objects
	mySprite = new sprite(myAtlas);
	entityList.push_back(mySprite);
	mySprite->copyFrame(uvData);


	// setting up the viewing window
	myRenderingSystem->setViewingWindow(WINDOW_WIDTH, WINDOW_HEIGHT, -2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 1000.0f);
	myShader[0].useProgram();
	kengine::matrix p = myRenderingSystem->getProjection();
	myShader[0].setProjection(p);

	myShader[1].useProgram();
	myShader[1].setProjection(p);

	// setting up camera position
	camera.lookAt(
		{ 0.0f, 0.0f, -1.0f }, // camera position
		{ 0.0f, 0.0f,  0.0f } // camera looking at position
	);
}


void GameEventHandler::afterMainLoopEvent()
{
}


void GameEventHandler::finishBeforeEvent()
{
	myCore->getGameWindow()->destroy();
}


void GameEventHandler::finishAfterEvent()
{
	myCore->stopMainLoop();
}


void GameEventHandler::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);

	// process the user input
	// update the player according to the input
	// update the state any AI
	// move NPC according to their AI

	for (auto e : entityList)
	{
		e->update(frameTime);
	}

	// colision detection between all game objects
	// colision reaction

	// update the camera
	camera.update(frameTime);
	kengine::matrix c = camera.get();

	// update the HUD

	// draw the scene
	//static const float bkgColor[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	static const float bkgColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bkgColor);

	kengine::matrix v(1);

	// updating entities
	int index = 0;

	for (auto e : entityList)
	{
		e->update(frameTime);

		kengine::matrix m(1.0f);

		v = c * m;

		// --------------------------------------------------------------------

		// copying model view data from entities
		for (int j = 0; j < 16; j++)
		{
			modelViewData[index++] = v.value()[j];
		}

		// copying uv from entitties
		mySprite->copyFrame(uvData);
	}

	// draw objects
	myShader[0].useProgram();

	myBatch->updateModelView(entityList.size(), modelViewData);
	myBatch->updateUV(TOTAL_ENTITY, uvData);
	myBatch->draw(static_cast<int>(entityList.size()));

	myShader[1].useProgram();

	primPoint->update(1 * 4, pointsData);
	primPoint->draw(1);

	primLine->update(1 * 8, linesData);
	primLine->draw(2);
}


void GameEventHandler::resumeEvent()
{
}


void GameEventHandler::pauseEvent()
{
}


void GameEventHandler::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);

	//if (state == K_MOUSE_DOWN)
	//{
	//	camera.setNavigation(button, x, y);
	//}
	//
	//if (state == K_MOUSE_UP)
	//{
	//	camera.clearNavigation();
	//}
}


void GameEventHandler::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);

	//camera.updateNavigation(x, y);
}


void GameEventHandler::keyboardEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);

	if (key == 27 && state)
	{
		myCore->getGameWindow()->destroy();
	}

	if (key == '1' && state)
	{
		myAtlas->copyFrame(0, uvData);
		myBatch->updateUV(TOTAL_ENTITY, uvData);
	}

	if (key == '2' && state)
	{
		myAtlas->copyFrame(1, uvData);
		myBatch->updateUV(TOTAL_ENTITY, uvData);
	}

	if (key == '0' && state)
	{
		mySprite->start();
	}

	if (key == '5' && state)
	{
		primPoint->setPointSize(10.0f);
		primLine->setLineWidth(5.0f);
	}

	if (key == '6' && state)
	{
		primPoint->setPointSize(1.0f);
		primLine->setLineWidth(1.0f);
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

	if (myShader)
	{
		myRenderingSystem->setViewport(0, 0, width, height);

		myRenderingSystem->setViewingWindow(width, height);
		kengine::matrix p = myRenderingSystem->getProjection();
		myShader[0].useProgram();
		myShader[0].setProjection(p);
		myShader[1].useProgram();
		myShader[1].setProjection(p);
	}
}


void GameEventHandler::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}