/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#ifndef K_ENGINE_DEMO_HPP
#define K_ENGINE_DEMO_HPP

#include <core.hpp>
#include <profile.hpp>

namespace game
{
	class demo : public kengine::events_callback
	{
	public:
		explicit demo(kengine::core* engine);

		void start();

		/*
			Callback event called before the main loop event starts
		*/
		void beforeMainLoopEvent();

		/*
			Callback event called once per main loop iteration
			@param - *frameTime* is a high resolution (<1us) time value
		*/
		void update(int64_t frameTime);

		/*
			Callback event called before the finish event starts
		*/
		void beforeFinishEvent();

		/*
			Callback event called after the finish event ends
		*/
		void afterFinishEvent();

		/*
			Callback event called when the window is activated
		*/
		void onResumeEvent();

		/*
			Callback event called when the window is deactivated
		*/
		void onPauseEvent();

		/*
			Callback event called when the window is being resized
		*/
		void onResizeWindowEvent(int width, int height);

		/*
			Callback event called when the window is being moved
		*/
		void onMoveWindowEvent(int x, int y);

		/*
			Callback event called on mouse move event
		*/
		void onMouseMotionEvent(int x, int y);

		/*
			Callback event called on mouse button event
		*/
		void onMouseButtonEvent(int button, kengine::MOUSE_ACTION state, int x, int y);

		/*
			Callback event called on keyboard key event
		*/
		void onKeyboardEvent(unsigned long long key, int state);

		/*
			Callback event called on special keyboard keys event like shift, ctrl, etc
		*/
		void onKeyboardSpecialEvent(unsigned long long key, int state);

		/*
			Callback event for debug message
		*/
		void debugMessage(const std::string& msg);

	private:
		kengine::core* m_engine;
		kengine::profile m_profile;
	};
}

#endif