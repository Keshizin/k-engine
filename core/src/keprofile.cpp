/*
	K-Engine Profile
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

#include <keprofile.h>
#include <ketimer.h>
#include <keaux.h>

#include <iostream>
#include <fstream>

/*
*
*  kengine::profile class - member class definition
* 
*/
kengine::profile::profile()
	:
		timer{ nullptr },
		framesPerSecond{ 0 },
		frameTime{ 0 },
		maxFrameTime{ 0 },
		minFrameTime{ 0 },
		meanFrameTime{ 0.0 },
		maxFramesPerSecond { 0 },
		minFramesPerSecond{ 0 },
		framesCounter{ 0 },
		frameTimeTotal{ 0 }
{
	timer = new kengine::timer;
	timer->setTimerInMs(1000);
}


kengine::profile::~profile()
{
	delete timer;
}


// copy constructor
kengine::profile::profile(const kengine::profile& copy)
	:
		timer{ nullptr },
		framesPerSecond {copy.framesPerSecond},
		frameTime { copy.frameTime },
		maxFrameTime{ copy.maxFrameTime },
		minFrameTime{ copy.minFrameTime },
		meanFrameTime{ copy.meanFrameTime },
		maxFramesPerSecond{ copy.maxFramesPerSecond },
		minFramesPerSecond{ copy.minFramesPerSecond },
		framesCounter {copy.framesCounter},
		frameTimeTotal{ copy.frameTimeTotal }
{
	timer = new kengine::timer(*copy.timer);
}


// copy assignment
kengine::profile& kengine::profile::operator=(const profile& copy)
{
	if(timer != nullptr)
		delete timer;

	framesPerSecond = copy.framesPerSecond;
	frameTime = copy.frameTime;
	framesCounter = copy.framesCounter;
	maxFrameTime = copy.maxFrameTime;
	minFrameTime = copy.minFrameTime;
	meanFrameTime = copy.meanFrameTime;
	frameTimeTotal = copy.frameTimeTotal;
	maxFramesPerSecond = copy.maxFramesPerSecond;
	minFramesPerSecond = copy.minFramesPerSecond;
	timer = new kengine::timer(*copy.timer);
	return *this;
}


void kengine::profile::start()
{
	framesPerSecond = 0;
	framesCounter = 0;
	maxFrameTime = 0;
	minFrameTime = 0;
	meanFrameTime = 0.0;
	frameTimeTotal = 0;
	timer->start();
}


bool kengine::profile::update(long long frameTimeParam)
{
	framesCounter++;
	frameTime = frameTimeParam;
	frameTimeTotal += frameTime;
	
	if(frameTime > maxFrameTime)
		maxFrameTime = frameTime;
	
	if(!minFrameTime || frameTime < minFrameTime)
		minFrameTime = frameTime;

	if (timer->isDoneAndRestart())
	{
		framesPerSecond = framesCounter;

		meanFrameTime = static_cast<double>(frameTimeTotal) / static_cast<double>(framesPerSecond);

		if(maxFramesPerSecond < framesPerSecond)
			maxFramesPerSecond = framesPerSecond;

		if(!minFramesPerSecond || minFramesPerSecond > framesPerSecond)
			minFramesPerSecond = framesPerSecond;

		framesCounter = 0;
		frameTimeTotal = 0;

		return true;
	}

	return false;
}


void kengine::profile::print() const
{
	std::cout
		<< "> kengine::core profile"   << "\n"
		<< "                    fps: " << framesPerSecond    << "\n"
		<< "                max fps: " << maxFramesPerSecond << "\n"
		<< "                min fps: " << minFramesPerSecond << "\n"
		<< "             frame time: " << frameTime          << "\n"
		<< "         max frame time: " << maxFrameTime       << "\n"
		<< "         min frame time: " << minFrameTime       << "\n"
		<< "        mean frame time: " << meanFrameTime      << "\n"
		<< std::endl;
}


/*
*
*  kengine::log class - members class definition
* 
* ----------------------------------------------------------------------------
*/

kengine::profile_log::profile_log(size_t size)
	: profiles(size), index{ 0 }
{
}


kengine::profile_log::~profile_log()
{
}


void kengine::profile_log::print()
{
	int i = 0;

	for (const kengine::profile& p : profiles)
	{
		std::cout << "> [profile log] | frame[ " << i++ << "]: " << std::endl;
		p.print();
	}
}


void kengine::profile_log::copy(const kengine::profile& copy)
{
	if (index < profiles.size() && index < MAX_PROFILES)
	{
		profiles.push_back(copy);
	}
}


void kengine::profile_log::writeToFile()
{
	std::ofstream logFile("kprofile.log");

	if (logFile.is_open())
	{
		int i = 0;

		for (const auto& p : profiles)
		{
			logFile << "> [profile log] | profile[" << i++ << "]:" << std::endl;
			logFile << "> [profile log] |             fps: " << p.framesPerSecond << std::endl;
			logFile << "> [profile log] |         max fps: " << p.maxFramesPerSecond << std::endl;
			logFile << "> [profile log] |         min fps: " << p.minFramesPerSecond << std::endl;
			logFile << "> [profile log] |      frame time: " << p.frameTime << std::endl;
			logFile << "> [profile log] |  max frame time: " << p.maxFrameTime << std::endl;
			logFile << "> [profile log] |  min frame time: " << p.minFrameTime << std::endl;
			logFile << "> [profile log] | mean frame time: " << p.meanFrameTime << std::endl;
			logFile << std::endl;
		}

		logFile.close();
	}
}