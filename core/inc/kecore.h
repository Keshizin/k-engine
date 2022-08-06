/*
	K-Engine Core
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

#ifndef K_ENGINE_CORE_H
#define K_ENGINE_CORE_H

#include <keeventhandler.h>
#include <kewinapiwrapper.h>
#include <kewindow.h>

#include <ketimehandler.h>
#include <keprofile.h>
#include <keconstants.h>

#include <kerenderingsystem.h>

namespace kengine
{
	// ------------------------------------------------------------------------
	//  kengine::core class
	// ------------------------------------------------------------------------
	class core
	{
	public:
		core();
		explicit core(kengine::eventhandler* evt);
		~core();

		core(const core& copy) = delete; // copy constructor
		core(core&& move) noexcept = delete; // move constructor
		core& operator=(const core& copy) = delete; // copy assignment

		void startMainLoop();
		void stopMainLoop();
		void pauseGameLoop();
		void resumeGameLoop();
		void setFrameRate(unsigned int framePerSecond);

		void setDefaultEventHandler();
		void setEventHandler(kengine::eventhandler* evt);

		kengine::win32wrapper* getWin32api() const { return win32api; }
		kengine::window* const getGameWindow() const;
		kengine::log* getProfileLog() { return &profileLog; }
		kengine::renderingsystem* getRenderingSystem() { return renderingSystem; }

	private:
		int runningStatus;
		kengine::eventhandler* eventHandler;
		kengine::win32wrapper* win32api;
		kengine::window* gameWindow;
		kengine::timehandler timeHandler;
		kengine::log profileLog;
		kengine::renderingsystem* renderingSystem;
	};
}

#endif