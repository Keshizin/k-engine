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

// ------------------------------------------------------------------------
//  SplashScene class
// ------------------------------------------------------------------------
game::SplashScene::SplashScene(kengine::core* engine, kengine::scene_manager* sceneManager)
	: scene{ engine, sceneManager }, windowWidth{ 640 }, windowHeight{ 480 }
{
}


game::SplashScene::~SplashScene()
{
}


void game::SplashScene::set()
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
		K_WINDOW_SPLASH);

	engineHandle->setFrameRate(0);

	kengine::renderingsystem* renderingSystemHandle = engineHandle->getRenderingSystem();

	renderingSystemHandle->setRenderContext(kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D);
	renderingSystemHandle->startup();
	renderingSystemHandle->setVSync(0);
	renderingSystemHandle->printInfo();

	engineHandle->getGameWindow()->show(SW_SHOW);
	engineHandle->startMainLoop();
}


void game::SplashScene::createWindowEvent() {}


void game::SplashScene::beforeMainLoopEvent()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	glClear(GL_COLOR_BUFFER_BIT);
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