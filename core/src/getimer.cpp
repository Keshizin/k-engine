/*
	Game Engine Timer
	This file is part of the K-Engine.

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

#include <getimer.h>

// ****************************************************************************
//  GETimer constructor and destructor
// ****************************************************************************
GETimer::GETimer(GETimeHandler *timeHandlerParam)
{
	this->timeHandler = timeHandlerParam;
	this->stopTime = 0;
	this->startTimer = 0;
	this->isStart = false;
}

// ****************************************************************************
//  GETimer public methods
// ****************************************************************************
void GETimer::setTimerInMs(long long stopTimeParam)
{
	this->stopTime = stopTimeParam * (timeHandler->getPerfomanceFrequency() / 1000);
}

void GETimer::start()
{
	startTimer = timeHandler->getInternalTimer();
	this->isStart = true;
}

void GETimer::restart(long long remainTime)
{
	startTimer = timeHandler->getInternalTimer() + remainTime;
}

void GETimer::stop()
{
	this->isStart = false;
}

int GETimer::isDone()
{
	long long internalTimer = timeHandler->getInternalTimer();

	if(this->isStart && internalTimer - startTimer >= stopTime)
		return 1;
	else
		return 0;
}

int GETimer::isRestart()
{
	long long internalTimer = timeHandler->getInternalTimer();

	if(this->isStart && internalTimer - startTimer >= stopTime)
	{
		// (!) dont put code before the next instruction!
		restart(internalTimer - startTimer - stopTime);
		return 1;
	}
	else
		return 0;
}