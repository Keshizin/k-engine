/*
	Game Engine Time Handler
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

#include <getimehandler.h>

GETimeHandler::GETimeHandler()
	: frameTime(0), internalTimer(0), frameTimeLimit(0), perfomanceFrequency(0)
{
}

// ****************************************************************************
//  GETimeHandler - Public Methods Definition
// ****************************************************************************
double GETimeHandler::getFrameTimeInSeconds() const
{
	return static_cast<double>(frameTime) / static_cast<double>(perfomanceFrequency);
}

void GETimeHandler::updateInternalTimer(long long timer)
{
	this->internalTimer = timer;
}

// ****************************************************************************
//  GETimeHandler - Getters and Setters Definition
// ****************************************************************************
void GETimeHandler::setFrameTime(long long frameTimeParam)
{
	this->frameTime = frameTimeParam;
}

long long GETimeHandler::getFrameTime() const
{
	return frameTime;
}

void GETimeHandler::setInternalTimer(long long internalTimerParam)
{
	this->internalTimer = internalTimerParam;
}

long long GETimeHandler::getInternalTimer() const
{
	return internalTimer;
}

void GETimeHandler::setFrameTimeLimit(long long frameTimeLimitParam)
{
	this->frameTimeLimit = frameTimeLimitParam;
}

long long GETimeHandler::getFrameTimeLimit() const
{
	return frameTimeLimit;
}

void GETimeHandler::setPerfomanceFrequency(long long perfomanceFrequencyParam)
{
	this->perfomanceFrequency = perfomanceFrequencyParam;
}

long long GETimeHandler::getPerfomanceFrequency() const
{
	return this->perfomanceFrequency;
}