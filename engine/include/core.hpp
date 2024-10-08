/*
	K-Engine Core
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

#ifndef K_ENGINE_CORE_HPP
#define K_ENGINE_CORE_HPP

#include <k_version.hpp>
#include <os_api_wrapper.hpp>
#include <events_callback.hpp>

namespace kengine
{
	/*
		k-engine runtime states
	*/
	enum class K_RUNTIME_STATE
	{
		RUNNING,
		STOPPED,
		PAUSED
	};

	/*
		kengine::core class orchestrates all core engine components
	*/
	class core
	{
	public:
		core();
		explicit core(events_callback* eventsCallback);
		~core();

		core(const core& copy) = delete; // copy constructor
		core& operator=(const core& copy) = delete; // copy assignment
		core(core&& move) noexcept = delete;  // move constructor
		core& operator=(core&&) = delete; // move assigment

		void startMainLoop();
		void stopMainLoop();
		void pauseGameLoop();
		void resumeGameLoop();

		void setEventsCallback(events_callback* eventsCallback);

	private:
		K_RUNTIME_STATE mainLoopState;
		events_callback* userEventsCallback;
	};

	/*
		Print platform data type sizes
	*/
	std::string infoType();
}

#endif