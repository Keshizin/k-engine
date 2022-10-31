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

#include <splash.h>
#include <keaux.h>
#include <keraw.h>
#include <game_constants.h>


game::SplashScene::SplashScene(kengine::core* engine, kengine::scene_manager* sceneManager)
	: scene{ engine, sceneManager },
	windowWidth{ 640 },
	windowHeight{ 480 },
	circlePoints{ nullptr },
	splashShader{},
	logoNode{},
	logoTexture{},
	circleTimer{},
	sceneTimer{}
{
}


game::SplashScene::~SplashScene()
{
	delete[] circlePoints;
}


void game::SplashScene::set()
{
	if (engineHandle == nullptr || sceneManagerHandle == nullptr)
		return;

	scene::set();

	// setting up window
	engineHandle->getGameWindow()->create(
		(MONITOR_WIDTH / 2) - (windowWidth / 2),
		(MONITOR_HEIGHT / 2) - (windowHeight / 2),
		windowWidth,
		windowHeight,
		"K-ENGINE DEMO",
		K_WINDOW_SPLASH);

	engineHandle->setFrameRate(0);

	// setting up the rendering system
	kengine::renderingsystem* renderingSystemHandle = engineHandle->getRenderingSystem();
	renderingSystemHandle->startup();
	renderingSystemHandle->setBlendingTest(1);
	renderingSystemHandle->setVSync(0);
	renderingSystemHandle->printInfo();

	engineHandle->getGameWindow()->show(SW_SHOW);

	// starting the main loop
	engineHandle->startMainLoop();
}


void game::SplashScene::createWindowEvent() {}


void game::SplashScene::beforeMainLoopEvent()
{
	glClearColor(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f); // gray background

	/*
	*  setting up the shaders
	*/
	kengine::ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "../../../../shaders/main.vert"},
		{GL_FRAGMENT_SHADER, "../../../../shaders/main.frag"},
		{GL_NONE, ""}
	};

	splashShader.loadShaders(shaders);
	//splashShader.print();

	/*
	*  setting up the camera
	*/
	cam.viewingInfo.set(windowWidth, windowHeight, -810.0f, 810.0f, -810.0f, 810.0f, 39.6f, 2.0f, 1000.0f);
	kengine::matrix projectionMatrix = cam.getProjectionMatrix();
	kengine::matrix viewMatrix(1);

	splashShader.useProgram();
	splashShader.setUniform("projectionMatrix", projectionMatrix);
	splashShader.setUniform("viewMatrix", viewMatrix);
	splashShader.setUniform("isTexture", 1);


	/*
	* 
	*  setting up the mesh and batch rendering
	* 
	*/
	kengine::mesh quad = kengine::quad(1080.0f);
	logoNode.load(quad, 1);

	kengine::raw_img img;
	img.loadfile("../../../../assets/shisap.kraw");
	logoTexture.load(img, 0);


	/*
	* 
	*  Setting up the timers and generate circle points for translating
	* 
	*/
	circlePoints = new float[(TOTAL_POINTS + 1) * 2];
	kengine::generateCirclePoints(TOTAL_POINTS + 1, circlePoints, 10.0f);

	circleTimer.setTimerInMs(20);
	circleTimer.start();

	sceneTimer.setTimerInMs(5000);
	sceneTimer.start();
}


void game::SplashScene::afterMainLoopEvent() {}


void game::SplashScene::finishBeforeEvent()
{
	scene::finishBeforeEvent();
}


void game::SplashScene::finishAfterEvent()
{
	scene::finishAfterEvent();
}


void game::SplashScene::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);

	if (sceneTimer.isDone())
	{
		engineHandle->getGameWindow()->destroy();
	}

	splashShader.useProgram();

	static int point_index = 0;

	if (circleTimer.isDoneAndRestart())
	{
		point_index += 2;

		if (point_index > TOTAL_POINTS * 2)
		{
			point_index = 0;
		}
	}

	kengine::matrix t_ = kengine::translate(circlePoints[point_index], circlePoints[point_index + 1], 0.0f);
	logoNode.updateModelMatrix(1, t_.value());

	glClear(GL_COLOR_BUFFER_BIT);
	logoNode.drawElements();
}


void game::SplashScene::resumeEvent() {}
void game::SplashScene::pauseEvent() {}


void game::SplashScene::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}


void game::SplashScene::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}


void game::SplashScene::keyboardEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);

	if (engineHandle == nullptr)
		return;

	if (key == K_KEY_ESCAPE && state == 1)
	{
		engineHandle->getGameWindow()->destroy();
	}

	if (key == K_KEY_RETURN && state == 1)
	{
		engineHandle->getGameWindow()->destroy();
	}
}


void game::SplashScene::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}


void game::SplashScene::resizeWindowEvent(int width, int height)
{
	K_UNREFERENCED_PARAMETER(width);
	K_UNREFERENCED_PARAMETER(height);
}


void game::SplashScene::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}
