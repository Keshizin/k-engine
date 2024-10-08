/*
	K-Engine profiling
	This file is part of the K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

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

#include <profile.hpp>
#include <os_api_wrapper.hpp>

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

kengine::profile::profile()
	:
	timer{ 1000 }, // one second timer
	framesPerSecond{},
	meanFrameTime{ 0.0 },
	maxFrameTime{ 0 },
	minFrameTime{ 0 },
	maxFramesPerSecond{ 0 },
	minFramesPerSecond{ 0 },
	frameCounter{ 0 },
	totalFrameTime{ 0 },
	isProfilingEnd{ false }
{
}

void kengine::profile::init()
{
	framesPerSecond.clear();
	meanFrameTime = 0.0;
	maxFrameTime = 0;
	minFrameTime = 0;
	maxFramesPerSecond = 0;
	minFramesPerSecond = 0;
	frameCounter = 0;
	totalFrameTime = 0;
	isProfilingEnd = false;
	timer.start();
}

void kengine::profile::end()
{
	isProfilingEnd = true;
}

void kengine::profile::update(int64_t frameTime)
{
	if (isProfilingEnd)
		return;

	//std::cout << "frametime: " << frameTime << std::endl;
	frameCounter++;
	totalFrameTime += frameTime;

	if (frameTime > maxFrameTime)
		maxFrameTime = frameTime;

	if (!minFrameTime || frameTime < minFrameTime)
		minFrameTime = frameTime;

	if (timer.doneAndRestart())
	{
		framesPerSecond.push_back(frameCounter);
		
		if (frameCounter > maxFramesPerSecond)
			maxFramesPerSecond = frameCounter;

		if (frameCounter < minFramesPerSecond)
			minFramesPerSecond = frameCounter;

		if (!minFramesPerSecond || frameCounter < minFramesPerSecond)
			minFramesPerSecond = frameCounter;

		meanFrameTime = static_cast<double>(totalFrameTime) / frameCounter;
		totalFrameTime = 0;

		std::cout << "> FPS: " << frameCounter << std::endl;
		frameCounter = 0;
	}
}

void kengine::profile::save() const
{
	std::ofstream logFile;

	logFile.open("profile.txt");

	auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	logFile << "> K-Engine Profile LOG - " << std::ctime(&currentTime) << std::endl;
	logFile << "> CLOCK FREQUENCY: " << kengine::getHighResolutionTimerFrequency() << std::endl;;
	logFile << "> MEAN FRAMETIME: " << meanFrameTime << std::endl;
	logFile << "> MAX FRAMETIME: " << maxFrameTime << std::endl;
	logFile << "> MIN FRAMETIME: " << minFrameTime << "\n" << std::endl;
	logFile << "> MAX FRAMES PER SECOND: " << maxFramesPerSecond << std::endl;
	logFile << "> MIN FRAMES PER SECOND: " << minFramesPerSecond << "\n" << std::endl;
	
	for (auto fps : framesPerSecond)
	{
		logFile << "- FPS: " << fps << std::endl;
	}

	logFile.close();
}