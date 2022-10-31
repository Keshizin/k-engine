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

#ifndef K_ENGINE_PROFILE_H
#define K_ENGINE_PROFILE_H

#include <vector>

namespace kengine
{
	class timer; // forward declaration

	/*
	* 
	*  kengine::profile class
	* 
	*/
	class profile
	{
		friend class profile_log;

	public:
		profile();
		~profile();

		profile(const profile& copy); // copy constructor
		profile& operator=(const profile& copy); // copy assignment

		void start();
		bool update(long long frameTime);
		void print() const;

	private:
		kengine::timer* timer;
		unsigned long long framesPerSecond;
		long long frameTime;
		long long maxFrameTime;
		long long minFrameTime;
		double meanFrameTime;
		unsigned long long maxFramesPerSecond;
		unsigned long long minFramesPerSecond;
		unsigned long long framesCounter;
		long long frameTimeTotal;
	};


	/*
	*
	*  kengine::log class
	* 
	*  This class consists of a vector of snapshots of runtime core engine
	*  information (i.e. frames per second, frame time, etc).
	* 
	*/
	class profile_log
	{
		static constexpr int MAX_PROFILES = 300;

	public:
		profile_log(size_t size);
		~profile_log();

		profile_log(const profile_log& copy) = delete; // copy constructor
		profile_log(profile_log&& move) noexcept = delete; // move constructor
		profile_log& operator=(const profile_log& copy) = delete; // copy assignment

		void restart() { index = 0; }
		void copy(const kengine::profile& copy);

		void print();
		void writeToFile();

	private:
		std::vector<kengine::profile> profiles;
		size_t index;
	};
}

#endif