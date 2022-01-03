/*
	K-Engine Core
	This file is part of the K-Engine.

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
#include <keeventhandler.h>
#include <ketimehandler.h>
#include <keprofile.h>

#include <iostream>

// ****************************************************************************
//  K-Engine Class - Constructors and Destructors
// ****************************************************************************
KEngine::KEngine(KEEventHandler *eventHandler)
	: apiWrapper(0), eventHandler(0), gameWindow(0), timeHandler(0), profile(0), runningStatus(K_STOPPED)
{
	this->apiWrapper = new KEWINAPIWrapper();
	setEventHandler(eventHandler);
	this->gameWindow = new KEWindow(this->apiWrapper);
	
	this->timeHandler = new KETimeHandler();
	timeHandler->setPerfomanceFrequency(apiWrapper->getHighResolutionTimerFrequency());

	this->profile = new KEProfile(this->apiWrapper);

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
	delete profile;
}

// ****************************************************************************
//  K-Engine Class - Public Methods
// ****************************************************************************
void KEngine::startMainLoop()
{
	long long startloopTime = 0;
	long long startTime = 0;
	long long endTime = 0;
	long long frameTime = 0;
	
	runningStatus = K_RUNNING;
	eventHandler->beforeMainLoopEvent();
	
	profile->start();
	startloopTime = endTime = apiWrapper->getHighResolutionTimerCounter();

	while(runningStatus != K_STOPPED)
	{
		startTime = apiWrapper->getHighResolutionTimerCounter();
		profile->update(timeHandler->getFrameTime());

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
			// renderingSystem->renderFrame();
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

	eventHandler->afterMainLoopEvent();
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
//  K-Engine Class - Getters and Setters
// ****************************************************************************
KEWINAPIWrapper* KEngine::getAPIWrapper() const
{
	return apiWrapper;
}

KEWindow* KEngine::getGameWindow() const
{
	return gameWindow;
}

KETimeHandler* KEngine::getTimeHandler() const
{
	return timeHandler;
}

KEProfile* KEngine::getProfile()
{
	return profile;
}

void KEngine::setEventHandler(KEEventHandler *eventHandlerParam)
{
	this->eventHandler = eventHandlerParam;
	this->apiWrapper->setGlobalEventHandler(this->eventHandler);
}