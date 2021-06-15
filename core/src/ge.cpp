/*
	Game Engine Core
	This file is part of the BPM Game Engine.

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

#include <ge.h>

// ****************************************************************************
//  KEngine - Constructors and Destructors
// ****************************************************************************
KEngine::KEngine(GEEventHandler *eventHandler)
	: apiWrapper(0), eventHandler(0), gameWindow(0), timeHandler(0), runningStatus(K_STOPPED)
{
	this->apiWrapper = new GEWINAPIWrapper();
	setEventHandler(eventHandler);

	this->gameWindow = new GEWindow(this->apiWrapper);

	this->timeHandler = new GETimeHandler();
	timeHandler->setPerfomanceFrequency(apiWrapper->getHighResolutionTimerFrequency());

	//this->renderingSystem = new GERenderingSystem(this->apiWrapper);
	//this->profile = new GEProfile(this->timeHandler);

#ifdef K_DEBUG
	// (!) only in debug mode!
	this->apiWrapper->createDebugConsole();
#endif
}

KEngine::~KEngine()
{
#ifdef K_DEBUG
	// (!) only in debug mode!
	this->apiWrapper->closeDebugConsole();
#endif

	delete apiWrapper;
	delete gameWindow;
	delete timeHandler;
	//delete renderingSystem;
	//delete profile;
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
void KEngine::startMainLoop()
{
	long long startloopTime = 0;
	long long startTime = 0;
	long long endTime = 0;
	long long frameTime = 0;
	
	runningStatus = K_RUNNING;
	timeHandler->setInternalTimer(0);
	eventHandler->beforeMainLoopEvent();

	// profile->start();
	startloopTime = endTime = apiWrapper->getHighResolutionTimerCounter();

	while(runningStatus != K_STOPPED)
	{
		startTime = apiWrapper->getHighResolutionTimerCounter();
		timeHandler->updateInternalTimer(startTime - startloopTime);
		// profile->update(timeHandler->getFrameTime());

		// --------------------------------------------------------------------
		//  Win32 Message Pump
		// --------------------------------------------------------------------
		apiWrapper->handleSystemMessages();

		// --------------------------------------------------------------------
		//  Start Game Loop!
		// --------------------------------------------------------------------
		if(runningStatus == K_RUNNING)
		{
			eventHandler->frameEvent(timeHandler->getFrameTimeInSeconds());

			//renderingSystem->renderFrame();
			apiWrapper->swapBuffers();
		}

		// --------------------------------------------------------------------
		//  End Game Loop!
		// --------------------------------------------------------------------
		frameTime = startTime - endTime;
		endTime = apiWrapper->getHighResolutionTimerCounter();
		frameTime += (endTime - startTime);

		// --------------------------------------------------------------------
		//  Frame Rate Governing
		// --------------------------------------------------------------------
		while(frameTime <= timeHandler->getFrameTimeLimit())
		{
			startTime = endTime;
			endTime = apiWrapper->getHighResolutionTimerCounter();
			frameTime += (endTime - startTime);
		}

		timeHandler->setFrameTime(frameTime);
	}
}

void KEngine::stopMainLoop()
{
	runningStatus = K_STOPPED;
}

void KEngine::pauseGameLoop()
{
	runningStatus = K_PAUSED;
}

void KEngine::resumeGameLoop()
{
	runningStatus = K_RUNNING;
}

void KEngine::setFrameRate(int framePerSecond)
{
	if (!framePerSecond)
	{
		timeHandler->setFrameTimeLimit(0);
	}
	else
	{
		timeHandler->setFrameTimeLimit(apiWrapper->getHighResolutionTimerFrequency() / framePerSecond);
	}	
}

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************
GEWINAPIWrapper* KEngine::getAPIWrapper() const
{
	return apiWrapper;
}

GEWindow *KEngine::getGameWindow() const
{
	return gameWindow;
}

//GERenderingSystem *KEngine::getRenderingSystem()
//{
//	return renderingSystem;
//}

GETimeHandler *KEngine::getTimeHandler()
{
	return timeHandler;
}

//GEProfile *KEngine::getProfile()
//{
//	return profile;
//}

void KEngine::setEventHandler(GEEventHandler *eventHandlerParam)
{
	this->eventHandler = eventHandlerParam;
	this->apiWrapper->setGlobalEventHandler(this->eventHandler);
}