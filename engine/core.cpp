/*
	K-Engine Core
	This file is part of the K-Engine.

	Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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
{
	osInitialize();
}

kengine::core::core(events_callback* eventsCallback)
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
	int64_t startTime;
	int64_t endTime = getHighResolutionTimerCounter();

	// please don't put code between endTime initialization and startTime initialization !!!

	while (mainLoopState != K_RUNTIME_STATE::STOPPED) {
		startTime = getHighResolutionTimerCounter();

		/*
			message pump
		*/

		handleSystemMessages();

		if (mainLoopState == K_RUNTIME_STATE::STOPPED)
			break;

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

void kengine::core::pauseMainLoop()
{
	mainLoopState = K_RUNTIME_STATE::PAUSED;
}

void kengine::core::resumeMainLoop()
{
	mainLoopState = K_RUNTIME_STATE::RUNNING;
}

void kengine::core::setEventsCallback(events_callback* eventsCallback)
{
	assert(!(eventsCallback == nullptr));
	userEventsCallback = eventsCallback;
	kengine::setGlobalUserEventsCallback(eventsCallback);
}

std::string kengine::getDataTypeInfo()
{
	return std::string("> Platform data types:") +
		"\n - int: " + std::to_string(sizeof(int)) +
		"\n - long int: " + std::to_string(sizeof(long int)) +
		"\n - int64_t: " + std::to_string(sizeof(int64_t)) +
		"\n";
}

std::string kengine::getVersion()
{
	return "kengine version " + std::to_string(K_ENGINE_VERSION_MAJOR) + "." + std::to_string(K_ENGINE_VERSION_MINOR) + "." + std::to_string(K_ENGINE_VERSION_PATCH);
}