/*
	K-Engine Core
	This file is part of the K-Engine.

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

kengine::scene defaultScene(nullptr, nullptr);

// ------------------------------------------------------------------------
//  kengine::core - members class definition
// ------------------------------------------------------------------------
kengine::core::core()
	:
	runningStatus{ K_STOPPED },
	eventHandler{ nullptr },
	win32api{ nullptr },
	gameWindow{ nullptr },
	timeHandler{},
	profileLog{ MAX_LOG_SIZE },
	renderingSystem{ nullptr }
{
#ifdef K_DEBUG
	createDebugConsole();
#endif

	defaultScene.setEngine(this);
	setEventHandler(&defaultScene);
	timeHandler.setPerfomanceFrequency(getHighResolutionTimerFrequency());

	win32api = new kengine::win32wrapper();
	gameWindow = new kengine::window(win32api);
	renderingSystem = new kengine::renderingsystem(win32api);
}


kengine::core::core(kengine::eventhandler* evt)
	:
		runningStatus{ K_STOPPED },
		eventHandler{ nullptr },
		win32api{ nullptr },
		gameWindow{ nullptr },
		timeHandler{},
		profileLog{ MAX_LOG_SIZE },
		renderingSystem{ nullptr }
{
#ifdef K_DEBUG
	createDebugConsole();
#endif

	setEventHandler(evt);
	timeHandler.setPerfomanceFrequency(getHighResolutionTimerFrequency());

	win32api = new kengine::win32wrapper();
	gameWindow = new kengine::window(win32api);
	renderingSystem = new kengine::renderingsystem(win32api);
}


kengine::core::~core()
{
#ifdef K_DEBUG
	// (!) only in debug mode!
	closeDebugConsole();
#endif

	delete renderingSystem;
	delete gameWindow;
	delete win32api;
	setEventHandler(nullptr);
}


void kengine::core::startMainLoop()
{
	if (eventHandler == nullptr)
		return;

	long long startTime = 0;
	long long endTime = 0;
	long long frameTime = 0;

	runningStatus = K_RUNNING;
	eventHandler->beforeMainLoopEvent();
	
	kengine::profile profile;
	profile.start();

	endTime = getHighResolutionTimerCounter();

	while (runningStatus != K_STOPPED)
	{
		startTime = getHighResolutionTimerCounter();

		// --------------------------------------------------------------------
		//  profiling stuff
		// --------------------------------------------------------------------
		if (profile.update(timeHandler.getFrameTime()))
		{
			profileLog.copy(profile);
		}

		// --------------------------------------------------------------------
		//  win32 message pump
		// --------------------------------------------------------------------
		handleSystemMessages();

		// --------------------------------------------------------------------
		//  start game loop
		// --------------------------------------------------------------------
		if (runningStatus == K_RUNNING)
		{
			eventHandler->frameEvent(timeHandler.getFrameTimeInSeconds());
			win32api->swapBuffers();
		}

		// --------------------------------------------------------------------
		//  end game loop
		// --------------------------------------------------------------------
		frameTime = startTime - endTime;
		endTime = getHighResolutionTimerCounter();
		frameTime += (endTime - startTime);

		// --------------------------------------------------------------------
		//  frame rate governing
		// --------------------------------------------------------------------
		while (frameTime <= timeHandler.getFrameTimeLimit())
		{
			startTime = endTime;
			endTime = getHighResolutionTimerCounter();
			frameTime += (endTime - startTime);
		}

		timeHandler.setFrameTime(frameTime);
	}

	eventHandler->afterMainLoopEvent();
}


void kengine::core::stopMainLoop()
{
	runningStatus = K_STOPPED;
}


void kengine::core::pauseGameLoop()
{
	runningStatus = K_PAUSED;
}


void kengine::core::resumeGameLoop()
{
	runningStatus = K_RUNNING;
}


void kengine::core::setFrameRate(unsigned int framePerSecond)
{
	if (!framePerSecond)
	{
		timeHandler.setFrameTimeLimit(0);
	}
	else
	{
		timeHandler.setFrameTimeLimit(getHighResolutionTimerFrequency() / framePerSecond);
	}
}


void kengine::core::setDefaultEventHandler()
{
	setEventHandler(&defaultScene);
}


void kengine::core::setEventHandler(kengine::eventhandler* evt)
{
	eventHandler = evt;
	kengine::setGlobalEventHandler(evt);
}


kengine::window* const kengine::core::getGameWindow() const
{
	return gameWindow;
}