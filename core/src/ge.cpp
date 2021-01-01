/*
	Game Engine Core
	This file is part of the BPM Game Engine.

	Copyright (C) 2020 Fabio Takeshi Ishikawa

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
#include <gewinapiwrapper.h>

// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
KEngine::KEngine(GEEventHandler *eventHandler)
{
	this->apiWrapper = new GEWINAPIWrapper();
	this->gameWindow = new GEWindow(this->apiWrapper);
	this->renderingSystem = new GERenderingSystem(this->apiWrapper);
	this->timeHandler = new GETimeHandler();

	setEventHandler(eventHandler);

	// (!) only in debug mode!
	this->apiWrapper->createDebugConsole();
}

KEngine::~KEngine()
{
	// (!) only in debug mode!
	this->apiWrapper->closeDebugConsole();

	delete apiWrapper;
	delete gameWindow;
	delete renderingSystem;
	delete timeHandler;
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
void KEngine::startMainLoop()
{
	unsigned long long startTime = 0;
	unsigned long long endTime = 0;
	unsigned long long frameTime = 0;

	runningStatus = K_RUNNING;
	timeHandler->setInternalTimer(0);
	timeHandler->setPerfomanceFrequency(apiWrapper->getHighResolutionTimerFrequency());

	eventHandler->beforeMainLoopEvent();

	while(runningStatus != K_STOPPED)
	{
		startTime = apiWrapper->getHighResolutionTimerCounter();
		timeHandler->updateInternalTimer();

		// --------------------------------------------------------------------
		//  Win32 Message Pump
		// --------------------------------------------------------------------
		apiWrapper->handleSystemMessages();

		// --------------------------------------------------------------------
		//  Start Game Loop!
		// --------------------------------------------------------------------
		if(runningStatus == K_RUNNING)
		{
			eventHandler->frameEvent();
			renderingSystem->renderFrame();
		}

		// --------------------------------------------------------------------
		//  End Game Loop!
		// --------------------------------------------------------------------
		frameTime = startTime - endTime;
		endTime = apiWrapper->getHighResolutionTimerCounter();
		frameTime += (endTime - startTime);

		// --------------------------------------------------------------------
		//  End Game Loop!
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
GEAPIWrapper *KEngine::getAPIWrapper()
{
	return apiWrapper;
}

GEWindow *KEngine::getGameWindow()
{
	return gameWindow;
}

GERenderingSystem *KEngine::getRenderingSystem()
{
	return renderingSystem;
}

void KEngine::setEventHandler(GEEventHandler *eventHandlerParam)
{
	this->eventHandler = eventHandlerParam;
	this->apiWrapper->setGlobalEventHandler(this->eventHandler);
}