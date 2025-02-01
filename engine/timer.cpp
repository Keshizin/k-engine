/*
	K-Engine Timer Class
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

#include <timer.hpp>
#include <os_api_wrapper.hpp>

kengine::timer::timer()
	:
	stopTime{ 0 },
	startTimer{ 0 },
	stopWatch{ 0 },
	frequency{ kengine::getHighResolutionTimerFrequency() }
{
}

kengine::timer::timer(long long stopTimeInMs)
	:
	stopTime{ stopTimeInMs * (kengine::getHighResolutionTimerFrequency() / 1000) },
	startTimer{ 0 },
	stopWatch{ 0 },
	frequency{ kengine::getHighResolutionTimerFrequency() }
{
}

void kengine::timer::start()
{
	stopWatch = stopTime;
	startTimer = getHighResolutionTimerCounter();
}

int kengine::timer::done()
{
	long long internalTimer = getHighResolutionTimerCounter();

	if ((internalTimer - startTimer) >= stopTime)
		return 1;
	else
		return 0;
}

void kengine::timer::stop()
{
	stopWatch = 0;
}

int kengine::timer::doneAndRestart()
{
	long long internalTimer = getHighResolutionTimerCounter();

	if (internalTimer - startTimer >= stopWatch)
	{
		// (!) dont put code before the next instruction!
		stopWatch += stopTime;
		return 1;
	}
	else
		return 0;
}

void kengine::timer::setTimerInMs(long long stopTimeInMs)
{
	stopTime = stopTimeInMs * (frequency / 1000); // convert from milliseconds to perfomance counter (high resolution < 1us)
}