/*
	K-Engine events callback
	This file is part of the K-Engine.

	Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_EVENTS_CALLBACK_HPP
#define K_ENGINE_EVENTS_CALLBACK_HPP

#include <k_constants.hpp>

#include <cstdint>
#include <string>

namespace kengine
{
	class window; // forward declaration

	/*
		This is an abstract class that contains user's callback methods that will be called by the kengine core.
	*/
	class events_callback
	{
	public:
		virtual ~events_callback() {}

		/*
			Callback event called before the main loop event starts
		*/
		virtual void beforeMainLoopEvent() = 0;

		/*
			Callback event called once per main loop iteration
			@param - *frameTime* is a high resolution (<1us) time value
		*/
		virtual void update(const int64_t frameTime) = 0;

		/*
			Callback event called when the apllication is being terminated
		*/
		virtual void onFinishEvent() = 0;

		/*
			Callback event called when the window is ready for use
		*/
		virtual void onWindowReady(window* window) = 0;

		/*
			Callback event called when the window is being destroyed
		*/
		virtual void onWindowDestroy() = 0;

		/*
			Callback event called when the window is deactivated (e.g. minimized window)
		*/
		virtual void onPauseEvent() = 0;

		/*
			Callback event called when the window is activated
		*/
		virtual void onResumeEvent() = 0;

		/*
			Callback event called when the user clicks the window's close button
		*/
		virtual void closeButtonEvent() = 0;

		/*
			Callback event called when the window is being resized
		*/
		virtual void onResizeWindowEvent(const int width, const int height) = 0;

		/*
			Callback event called when the window is being moved
		*/
		virtual void onMoveWindowEvent(const int x, const int y) = 0;

		/*
			Callback event called on mouse move event
		*/
		virtual void onMouseMotionEvent(const int x, const int y) = 0;

		/*
			Callback event called on mouse button event
		*/
		virtual void onMouseButtonEvent(const int button, const MOUSE_ACTION& state, const int x, const int y) = 0;

		/*
			Callback event called on keyboard key event
		*/
		virtual void onKeyboardEvent(const unsigned long long key, const int state) = 0;

		/*
			Callback event called on special keyboard keys event like shift, ctrl, etc
		*/
		virtual void onKeyboardSpecialEvent(const unsigned long long key, const int state) = 0;

		/*
			Callback event for debug message
		*/
		virtual void debugMessage(const std::string& msg) = 0;

		/*
			Custom message pump handler
			This callback is specific for Win32 window procedure
		*/
		virtual void customWindowProcedure(void* param1, void* param2, void* param3, void* param4) = 0;
	};
}

#endif