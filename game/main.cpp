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

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define TOTAL_ENTITY 1


// ----------------------------------------------------------------------------
//  Game Entity
// ----------------------------------------------------------------------------
class GameObject : public kengine::entity
{
public:
	GameObject() : translate{}, rotate{}, scale{} {}
	~GameObject() {}

	GameObject(const GameObject& copy) = delete; // copy constructor
	GameObject(GameObject&& move) noexcept = delete; // move constructor
	GameObject& operator=(const GameObject& copy) = delete; // copy assignment

	void update(double frameTime) { K_UNREFERENCED_PARAMETER(frameTime); }

private:
	kengine::transform<float> translate;
	kengine::transform<float> rotate;
	kengine::transform<float> scale;
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
kengine::instancedmodelnode* myInstancedNode;

// entity list and model view buffer object data
GameObject* entityList;
GLfloat* modelViewData;

// texture object
kengine::texture* myTexture;


// ----------------------------------------------------------------------------
//  Point Entry Execution
// ----------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);
	K_UNREFERENCED_PARAMETER(nCmdShow);

	GameEventHandler my_events;
	myCore = new kengine::core(&my_events);
	myCore->getGameWindow()->create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "K-ENGINE DEMO", K_WINDOW_COMPLETE);
	
	myRenderingSystem = new kengine::renderingsystem(myCore->getWin32api());
	myRenderingSystem->startup();
	myRenderingSystem->setVSync(0);
	
	myCore->getGameWindow()->show(nCmdShow);
	myCore->setFrameRate(0);
	myCore->startMainLoop();

	delete myTexture;
	delete[] modelViewData;
	delete[] entityList;
	delete myInstancedNode;
	delete myShader;
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
	myRenderingSystem->printInfo();
	myRenderingSystem->setPolygonMode(K_RENDERING_MODE_FILL);

	myShader = new kengine::TransformProgram;

	kengine::ShaderInfo shaders[] = {
			{GL_VERTEX_SHADER, "../../../../shaders/main_vertex_shader.vert"},
			{GL_FRAGMENT_SHADER, "../../../../shaders/main_fragment_shader.frag"},
			{GL_NONE, ""}
	};

	myShader->loadShaders(shaders);

	kengine::model m = kengine::quad(1.0f);
	myInstancedNode = new kengine::instancedmodelnode(TOTAL_ENTITY, m);

	kengine::raw_img img;
	img.loadfile("../../../../shaders/shisa.png.KRAW");
	myTexture = new kengine::texture(img);
	myTexture->bindTexture(0, 1);

	entityList = new GameObject[TOTAL_ENTITY];
	modelViewData = new GLfloat[TOTAL_ENTITY * 16];
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

	static const float bkgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bkgColor);

	// updating entities
	int index = 0;

	for (int i = 0; i < TOTAL_ENTITY; i++)
	{
		entityList[i].update(frameTime);
		kengine::matrix m(1.0f);

		for (int j = 0; j < 16; j++)
		{
			modelViewData[index++] = m.value()[j];
		}
	}

	// draw objects
	myShader->useProgram();
	kengine::matrix p = kengine::ortho(-2.0f, 2.0f, -2.0, 2.0f, -1.0f, 1.0f);
	myShader->setProjection(p);
	myInstancedNode->update(TOTAL_ENTITY, modelViewData);
	myInstancedNode->draw(TOTAL_ENTITY);
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

	if(key == 27 && state)
	{
		myCore->getGameWindow()->destroy();
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
	myRenderingSystem->setViewport(0, 0, width, height);
}


void GameEventHandler::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}