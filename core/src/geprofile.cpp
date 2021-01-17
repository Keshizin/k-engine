/*
	Game Engine Profile
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

#include <geprofile.h>

// #include <iostream>

// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
GEProfile::GEProfile(GETimeHandler *timeHandler)
{
	framesPerSecond = 0;
	timer = new GETimer(timeHandler);
	frameTimeCounter = 0;
	maxFrameTime = 0;
	minFrameTime = 0;
	meanFrameTime = 0.0;
}

GEProfile::~GEProfile()
{
	delete timer;
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
void GEProfile::start()
{
	framesPerSecond = 0;
	framesCounter = 0;
	timer->setTimerInMs(1000); // 1000 milliseconds
	timer->startLoop(0);
	frameTimeCounter = 0;
	maxFrameTime = 0;
	minFrameTime = 0;
	meanFrameTime = 0.0;
}

void GEProfile::update(long long frameTime)
{
	framesCounter++;

	if(frameTime > maxFrameTime)
		maxFrameTime = frameTime;

	if(!minFrameTime || frameTime < minFrameTime)
		minFrameTime = frameTime;

	frameTimeCounter += frameTime;

	if(timer->isDoneLoop())
	{
		framesPerSecond = framesCounter;
		meanFrameTime = static_cast<double>(frameTimeCounter) / static_cast<double>(framesPerSecond);

		// std::cout << "fps: " << framesPerSecond << std::endl;
		// std::cout << "meanFrameTime: " << meanFrameTime << std::endl;
		// std::cout << "maxFrameTime: " << maxFrameTime << std::endl;
		// std::cout << "minFrameTime: " << minFrameTime << std::endl << std::endl;

		frameTimeCounter = 0;
		framesCounter = 0;
		maxFrameTime = 0;
		minFrameTime = 0;
	}
}

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************
unsigned long long GEProfile::getFramesPerSecond()
{
	return framesPerSecond;
}

long long GEProfile::getMaxFrameTime()
{
	return maxFrameTime;
}

long long GEProfile::getMinFrameTime()
{
	return minFrameTime;
}

double GEProfile::getMeanFrameTime()
{
	return meanFrameTime;
}