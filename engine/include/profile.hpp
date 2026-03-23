/*
	K-Engine profiling
	This file is part of the K-Engine.

	Copyright (C) 2020-2026 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_PROFILE_HPP
#define K_ENGINE_PROFILE_HPP

#include <timer.hpp>
#include <vector>
#include <cstdint>

namespace kengine
{
	class profile
	{
	public:
		profile();

		void init();
		void end();
		void update(int64_t frameTime);
		void save() const;

		int getFramesPerSecond() const { return !framesPerSecond.empty() ? framesPerSecond.back() : 0; };

	private:
		kengine::timer timer;
		std::vector<int> framesPerSecond;

		double meanFrameTime = 0.0;
		int64_t maxFrameTime = 0;
		int64_t minFrameTime = 0;
		int64_t totalFrameTime = 0;
		int maxFramesPerSecond = 0;
		int minFramesPerSecond = 0;
		int frameCounter = 0;
		bool isProfilingEnd = false;;
	};
}

#endif