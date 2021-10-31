/*
	K-Engine Timer Class
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

#include <ketimer.h>
#include <kewinapiwrapper.h>
// #include <iostream>

// ****************************************************************************
//  KETimer constructor and destructor
// ****************************************************************************
KETimer::KETimer(KEWINAPIWrapper *apiWrapper)
	: stopTime(0), startTimer(0), temp(0), isStart(false), apiWrapper(apiWrapper)
{
}

// ****************************************************************************
//  KETimer Public Methods
// ****************************************************************************
void KETimer::setTimerInMs(long long stopTimeParam)
{
	this->stopTime = stopTimeParam * (apiWrapper->getHighResolutionTimerFrequency() / 1000);
}

void KETimer::start()
{
	startTimer = apiWrapper->getHighResolutionTimerCounter();
	this->temp = startTimer;
	this->isStart = true;
}

int KETimer::isDone()
{
	long long internalTimer = apiWrapper->getHighResolutionTimerCounter();

	if(this->isStart && internalTimer - startTimer >= stopTime)
	{
		// std::cout << "---------------------------------" << std::endl;
		// std::cout << "@debug | isDone | startTimer: " << startTimer << std::endl;
		// std::cout << "@debug | isDone | internalTimer: " << internalTimer << std::endl;
		// std::cout << "---------------------------------" << std::endl;
		return 1;
	}
	else
		return 0;
}

void KETimer::stop()
{
	this->isStart = false;
}

int KETimer::isDoneAndRestart()
{
	long long internalTimer = apiWrapper->getHighResolutionTimerCounter();

	if(this->isStart && internalTimer - startTimer >= stopTime)
	{
		// (!) dont put code before the next instruction!
		startTimer = internalTimer + (internalTimer - startTimer - stopTime);

		// std::cout << "---------------------------------" << std::endl;
		// std::cout << "@debug | isDone | startTimer: " << this->temp << std::endl;
		// std::cout << "@debug | isDone | internalTimer: " << internalTimer << std::endl;
		// std::cout << "---------------------------------" << std::endl;
		// this->temp = startTimer;
		return 1;
	}
	else
		return 0;
}