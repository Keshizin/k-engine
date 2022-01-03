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

#include <keprofile.h>
#include <ketimer.h>
#include <kewinapiwrapper.h>
#include <iostream>

// ****************************************************************************
//  KEProfile - Constructors & Destructors
// ****************************************************************************
KEProfile::KEProfile(KEWINAPIWrapper *apiWrapper) :
	timer(0),
	framesPerSecond(0),
	framesCounter(0),
	maxFrameTime(0),
	minFrameTime(0),
	meanFrameTime(0),
	frameTimeTotal(0),
	maxFramesPerSecond(0),
	minFramesPerSecond(0)
{
	timer = new KETimer(apiWrapper);
	timer->setTimerInMs(1000);
}

KEProfile::~KEProfile()
{
	delete timer;
}

// ****************************************************************************
//  KEProfile Class - Public Methods
// ****************************************************************************
void KEProfile::start()
{
	framesPerSecond = 0;
	framesCounter = 0;
	timer->start();
	maxFrameTime = 0;
	minFrameTime = 0;
	meanFrameTime = 0.0;
	frameTimeTotal = 0;
	minFramesPerSecond = 0;
	maxFramesPerSecond = 0;
}

void KEProfile::update(long long frameTime)
{
	if(timer->isDoneAndRestart())
	{
		meanFrameTime = static_cast<double>(frameTimeTotal) / static_cast<double>(framesPerSecond);
		framesPerSecond = framesCounter;

		if(maxFramesPerSecond < framesPerSecond)
			maxFramesPerSecond = framesPerSecond;

		if(!minFramesPerSecond || minFramesPerSecond > framesPerSecond)
			minFramesPerSecond = framesPerSecond;

		std::cout
			<< "FPS: " << framesPerSecond
			<< "\nMAX FPS: " << maxFramesPerSecond
			<< "\nMIN FPS: " << minFramesPerSecond
			<< "\nMEAN FRAMETIME: " << meanFrameTime
			<< "\nMAX FRAMETIME: " << maxFrameTime
			<< "\nMIN FRAMETIME: " << minFrameTime
			<< "\n" << std::endl;

		framesCounter = 0;
		frameTimeTotal = 0;
	}
	else
	{
		framesCounter++;
		frameTimeTotal += frameTime;

		if(frameTime > maxFrameTime)
			maxFrameTime = frameTime;

		if(!minFrameTime || frameTime < minFrameTime)
			minFrameTime = frameTime;
	}
}

// ****************************************************************************
//  KEProfile Class - Getters and Setters
// ****************************************************************************
unsigned long long KEProfile::getFramesPerSecond() const
{
	return framesPerSecond;
}

unsigned long long KEProfile::getMaxFramesPerSecond() const
{
	return maxFramesPerSecond;
}

unsigned long long KEProfile::getMinFramesPerSecond() const
{
	return minFramesPerSecond;
}

long long KEProfile::getMaxFrameTime() const
{
	return maxFrameTime;
}

long long KEProfile::getMinFrameTime() const
{
	return minFrameTime;
}

double KEProfile::getMeanFrameTime() const
{
	return meanFrameTime;
}