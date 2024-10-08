/*
	K-Engine Core
	This file is part of the K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

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

#include <core.hpp>
#include <iostream>
#include <cassert>

kengine::core::core()
	:
	mainLoopState{ K_RUNTIME_STATE::STOPPED },
	userEventsCallback{ nullptr }
{
	osInitialize();
}

kengine::core::core(events_callback* eventsCallback)
	:
	mainLoopState{ K_RUNTIME_STATE::STOPPED },
	userEventsCallback { nullptr }
{
	setEventsCallback(eventsCallback);
	osInitialize();
}

kengine::core::~core()
{
	osFinish();
}

void kengine::core::startMainLoop()
{
	assert(!(userEventsCallback == nullptr));

	mainLoopState = K_RUNTIME_STATE::RUNNING;

	int64_t frameTime = 0;
	//int64_t frequency = getHighResolutionTimerFrequency();

	userEventsCallback->beforeMainLoopEvent();
	int64_t endTime = getHighResolutionTimerCounter();

	// please don't put code between endTime initialization and startTime initialization !!!

	while (mainLoopState != K_RUNTIME_STATE::STOPPED)
	{
		int64_t startTime = getHighResolutionTimerCounter();

		/*
			message pump
		*/

		handleSystemMessages();

		/*
			user's update
		*/

		userEventsCallback->update(frameTime);


		/*
			end of main loop
		*/

		frameTime = startTime - endTime;
		endTime = getHighResolutionTimerCounter();
		frameTime += (endTime - startTime);
	}
}

void kengine::core::stopMainLoop()
{
	mainLoopState = K_RUNTIME_STATE::STOPPED;
}

void kengine::core::pauseGameLoop()
{
	mainLoopState = K_RUNTIME_STATE::PAUSED;
}

void kengine::core::resumeGameLoop()
{
	mainLoopState = K_RUNTIME_STATE::RUNNING;
}

void kengine::core::setEventsCallback(events_callback* eventsCallback)
{
	assert(!(eventsCallback == nullptr));
	userEventsCallback = eventsCallback;
	kengine::setGlobalUserEventsCallback(eventsCallback);
}

std::string kengine::infoType()
{
	return std::string("> k-engine data types:") +
		"\n\t- int: " + std::to_string(sizeof(int)) +
		"\n\t- long int: " + std::to_string(sizeof(long int)) +
		"\n\t- int64_t: " + std::to_string(sizeof(int64_t)) +
		"\n";
}