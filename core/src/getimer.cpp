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
GETimer::GETimer(GETimeHandler *timeHandler)
{
	this->timeHandler = timeHandler;
}

// ****************************************************************************
//  GETimer public methods
// ****************************************************************************
void GETimer::setTimerInMs(unsigned long long stopTime)
{
	if (stopTime < 1000)
		this->stopTime = stopTime * timeHandler->getPerfomanceFrequency();
	else
		this->stopTime = (stopTime / 1000) * timeHandler->getPerfomanceFrequency();
}

void GETimer::start()
{
	startTimer = timeHandler->getInternalTimer();
}

void GETimer::startLoop(unsigned long long remainTime)
{
	startTimer = timeHandler->getInternalTimer() + remainTime;
}

int GETimer::isDone()
{
	if(startTimer && timeHandler->getInternalTimer() - startTimer >= stopTime)
	{
		return 1;
	}
	else
		return 0;
}

int GETimer::isDoneLoop()
{
	unsigned long long internalTimer = timeHandler->getInternalTimer();

	if(startTimer && internalTimer - startTimer >= stopTime)
	{
		startLoop(internalTimer - startTimer - stopTime);
		return 1;
	}
	else
		return 0;
}