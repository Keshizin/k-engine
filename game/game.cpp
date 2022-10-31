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

#include <game.h>
#include <keaux.h>
#include <game_constants.h>


game::GameScene::GameScene(kengine::core* engine, kengine::scene_manager* sceneManager)
	: scene{ engine, sceneManager },
	windowWidth{ 1280 },
	windowHeight{ 720 },
	gameShaders{},
	gameObjectMesh{},
	planeMesh{},
	cam{}
{
}


game::GameScene::~GameScene()
{
}


void game::GameScene::set()
{
	if (engineHandle == nullptr)
		return;

	scene::set();
	
	engineHandle->getGameWindow()->create(
		(MONITOR_WIDTH / 2) - (windowWidth / 2),
		(MONITOR_HEIGHT / 2) - (windowHeight / 2),
		windowWidth,
		windowHeight,
		"K-ENGINE DEMO",
		K_WINDOW_COMPLETE);
	
	engineHandle->setFrameRate(0);

	kengine::renderingsystem* renderingSystemHandle = engineHandle->getRenderingSystem();

	renderingSystemHandle->startup();
	renderingSystemHandle->setPolygonMode(K_RENDERING_MODE_FILL);
	renderingSystemHandle->setBlendingTest(1);
	renderingSystemHandle->setDepthTest(1);
	renderingSystemHandle->setVSync(0);

	engineHandle->getGameWindow()->show(SW_SHOW);
	engineHandle->startMainLoop();
}


void game::GameScene::createWindowEvent()
{
}


void game::GameScene::beforeMainLoopEvent()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*
	* 
	*  setting up the shaders
	* 
	*/
	kengine::ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "../../../../shaders/phong.vert"},
		{GL_FRAGMENT_SHADER, "../../../../shaders/phong.frag"},
		{GL_NONE, ""}
	};

	gameShaders.loadShaders(shaders);
	//gameShaders[0].print();


	/*
	* 
	*  setting up the camera
	* 
	*/
	cam.renderContext = kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_FRUSTUM;
	cam.viewingInfo.window_width = windowWidth;
	cam.viewingInfo.window_height = windowHeight;
	kengine::matrix projectionMatrix = cam.getProjectionMatrix();

	gameShaders.useProgram();
	gameShaders.setUniform("projectionMatrix", projectionMatrix);


	/*
	* 
	*  setting up the lighting and shading
	* 
	*/
	kengine::phong_light light;
	light.type = kengine::SOURCE_LIGHT_TYPE::K_LIGHT_TYPE_SINGLE_POINT;
	light.position = { 0.0f, 30.0f, 0.0f };
	light.La = { 0.3f, 0.3f, 0.3f };
	light.Ld = { 0.8f, 0.2f, 0.3f };
	light.Ls = { 0.6f, 0.6f, 0.6f };
	light.spot_cutoff = { 90.0f * K_PI_TO_RADIAN };
	light.spot_direction = { 0.0f, 0.0f, 0.0f };
	light.spot_exponent = { 50.0f };

	kengine::phong_material mat;
	mat.Ka = { 0.3f, 0.3f, 0.3f };
	mat.Kd = { 0.8f, 0.2f, 0.2f };
	mat.Ks = { 0.7f, 0.7f, 0.7f };
	mat.shininess = 10.0f;

	kengine::lighting_model lightingModel;
	lightingModel.nonLocalViewer = 0;
	lightingModel.flatShading = 0;
	lightingModel.twoSidedShading = 0;
	lightingModel.modelType = kengine::LIGHTING_MODEL_TYPE::K_BLINN_PHONG_REFLECTION_MODEL;

	gameShaders.setUniform("lights[0]", light);
	gameShaders.setUniform("material", mat);
	gameShaders.setUniform("lightingModel", lightingModel);


	/*
	*
	*  setting up the mesh and batch rendering
	*
	*/
	kengine::mesh plane = kengine::kraw_load_obj("../../../../assets/plane.kraw");
	planeMesh.load(plane, 1);

	kengine::mesh torus = kengine::kraw_load_obj("../../../../assets/torus.kraw");
	gameObjectMesh.load(torus, 1);

	/*
	* 
	*  setting up the camera
	* 
	*/
	cam.eye.x = 0.0f;
	cam.eye.y = 35.0f;
	cam.eye.z = -50.0f;
	cam.lookAt();
}


void game::GameScene::afterMainLoopEvent() {}


void game::GameScene::finishBeforeEvent()
{
	scene::finishBeforeEvent();
}


void game::GameScene::finishAfterEvent()
{
	scene::finishAfterEvent();
}


void game::GameScene::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);

	static float angle = 0.0f;

	angle += 0.01f;
	
	if (angle >= 360.0f)
		angle = 0.0f;

	kengine::matrix r1 = kengine::rotate(angle, 1.0f, 0.0f, 0.0f);
	kengine::matrix r2 = kengine::rotate(angle, 0.0f, 1.0f, 0.0f);
	kengine::matrix r3 = kengine::rotate(angle, 0.0f, 0.0f, 1.0f);
	kengine::matrix t = kengine::translate(0.0f, 10.0f, 0.0f);
	kengine::matrix s = kengine::scale(5.0f, 5.0f, 5.0f);
	
	kengine::matrix m = t * s * r1 * r2 * r3;
	gameObjectMesh.updateModelMatrix(TOTAL_ENTITIES, m.value());

	gameShaders.useProgram();
	kengine::matrix view = cam.get();
	gameShaders.setUniform("viewMatrix", view);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	planeMesh.drawArrays();
	gameObjectMesh.drawArrays();
}


void game::GameScene::resumeEvent() {}
void game::GameScene::pauseEvent() {}


void game::GameScene::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);

	if (button == K_MOUSE_LEFT_BUTTON && state == K_MOUSE_DOWN)
	{
		//cam.setNavigation(button, x, y);
	}
	
	if (button == K_MOUSE_LEFT_BUTTON && state == K_MOUSE_UP)
	{
		//cam.clearNavigation();
	}
}


void game::GameScene::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);

	//cam.updateNavigation(x, y);
}


void game::GameScene::keyboardEvent(unsigned long long key, int state)
{
	if (key == K_KEY_ESCAPE && state == 1)
	{
		engineHandle->getGameWindow()->destroy();
	}

	if (key == '1' && state == 1)
	{
		cam.eye.x -= 1.0f;
		cam.lookAt();
	}

	if (key == '2' && state == 1)
	{
		cam.eye.x += 1.0f;
		cam.lookAt();
	}

	if (key == '3' && state == 1)
	{
		cam.eye.y += 1.0f;
		cam.lookAt();
	}

	if (key == '4' && state == 1)
	{
		cam.eye.y -= 1.0f;
		cam.lookAt();
	}

	if (key == '5' && state == 1)
	{
		cam.eye.z += 1.0f;
		cam.lookAt();
	}

	if (key == '6' && state == 1)
	{
		cam.eye.z -= 1.0f;
		cam.lookAt();
	}
}


void game::GameScene::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}


void game::GameScene::resizeWindowEvent(int width, int height)
{
	K_UNREFERENCED_PARAMETER(width);
	K_UNREFERENCED_PARAMETER(height);

	if (engineHandle != nullptr)
	{
		engineHandle->getRenderingSystem()->setViewport(0, 0, width, height);
		cam.viewingInfo.window_width = width;
		cam.viewingInfo.window_height = height;
		kengine::matrix p = cam.getProjectionMatrix();
		gameShaders.useProgram();
		gameShaders.setUniform("projectionMatrix", p);
	}
}


void game::GameScene::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}