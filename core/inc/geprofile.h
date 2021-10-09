/*
	K-Engine Profile
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

#ifndef K_ENGINE_PROFILE_H
#define K_ENGINE_PROFILE_H

#include <ketimer.h>
#include <ketimehandler.h>

// ****************************************************************************
//  Game Engine Profile Class
// ****************************************************************************
class GEProfile
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	GEProfile(KETimeHandler *timeHandler);
	~GEProfile();

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	void start();
	void update(long long frameTime);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	unsigned long long getFramesPerSecond();

	long long getMaxFrameTime();
	long long getMinFrameTime();
	double getMeanFrameTime();

private:
	KETimer *timer;
	unsigned long long framesPerSecond;
	unsigned long long framesCounter;
	long long maxFrameTime;
	long long minFrameTime;
	double meanFrameTime;
	long long frameTimeCounter;
	unsigned long long maxFramePerSecond;
	unsigned long long minFramePerSecond;
};

#endif