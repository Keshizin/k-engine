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
#include <iostream>

kengine::profile::profile(int  s)
	:
	timer{ 1000 }, // one second timer
	frameTimes{},
	framesPerSeconds{},
	framesPerSecond{0},
	frameCounter{ 0 },
	max_size{ s }
{
}

void kengine::profile::init()
{
	frameTimes.clear();
	framesPerSeconds.clear();
	frameCounter = 0;
	timer.start();
}

int kengine::profile::update(int64_t frameTime)
{
	frameTimes.push_back(frameTime);
	frameCounter++;

	if (timer.doneAndRestart())
	{
		framesPerSeconds.push_back(frameCounter);
		frameCounter = 0;
		frameTimes.push_back(-1);
		return 1;
	}

	return 0;
}

int kengine::profile::size() const
{
	return max_size;
}

void kengine::profile::print() const
{
	for (size_t index = 0; index < frameTimes.size(); index++)
	{
		std::cout << "[" << index << "] frame time : " << std::fixed << frameTimes[index] << std::endl;
	}

	for (size_t index = 0; index < framesPerSeconds.size(); index++)
	{
		std::cout << "[" << index << "] FPS : " << std::fixed << framesPerSeconds[index] << std::endl;
	}
}