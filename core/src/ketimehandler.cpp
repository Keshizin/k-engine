/*
	K-Engine Time Handler
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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

#include <ketimehandler.h>
#include <keaux.h>


// ----------------------------------------------------------------------------
//  timehandler class - constructor and destructor definition
// ----------------------------------------------------------------------------
kengine::timehandler::timehandler()
	: frameTime{ 0 }, frameTimeLimit{ 0 }, perfomanceFrequency{ 0 }
{
}


kengine::timehandler::~timehandler()
{
}


// ----------------------------------------------------------------------------
//  timehandler class - public methods definition
// ----------------------------------------------------------------------------
double kengine::timehandler::getFrameTimeInSeconds() const
{
	return static_cast<double>(frameTime) / static_cast<double>(perfomanceFrequency);
}


// ----------------------------------------------------------------------------
//  timehandler class - getters and setters definition
// ----------------------------------------------------------------------------
void kengine::timehandler::setFrameTime(long long frameTimeParam)
{
	frameTime = frameTimeParam;
}


long long kengine::timehandler::getFrameTime() const
{
	return frameTime;
}


void kengine::timehandler::setFrameTimeLimit(long long frameTimeLimitParam)
{
	frameTimeLimit = frameTimeLimitParam;
}


long long kengine::timehandler::getFrameTimeLimit() const
{
	return frameTimeLimit;
}


void kengine::timehandler::setPerfomanceFrequency(long long perfomanceFrequencyParam)
{
	perfomanceFrequency = perfomanceFrequencyParam;
}


long long kengine::timehandler::getPerfomanceFrequency() const
{
	return perfomanceFrequency;
}