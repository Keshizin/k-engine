/*
	K-Engine Demonstration
	This file provide a splash screen example created with K-Engine.

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

#ifndef K_ENGINE_DEMO_SPLASH_HPP
#define K_ENGINE_DEMO_SPLASH_HPP

#include <core.hpp>
#include <rendering_system.hpp>
#include <camera.hpp>

namespace demo
{
	class splash : public kengine::events_callback
	{
	public:
		explicit splash(kengine::core* engine);
		~splash();

		splash(const splash& copy) = delete; // copy constructor
		splash(splash&& move) noexcept = delete; // move constructor
		splash& operator=(const splash& copy) = delete; // copy assignment
		splash& operator=(splash&&) = delete; // move assigment

		void start();
		void createWindow();
		void showWindow();

		/*
			Callback event called when the window is ready for use
		*/
		void onWindowReady(kengine::window* window);

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
			Callback event called when the apllication is being terminated
		*/
		void onFinishEvent();

		/*
			Callback event called when the window is being destroyed
		*/
		void onWindowDestroy();

		/*
			Callback event called when the window is deactivated (e.g. minimized window)
		*/
		void onPauseEvent();

		/*
			Callback event called when the window is activated
		*/
		void onResumeEvent();

		/*
			Callback event called when the user clicks the window's close button
		*/
		void closeButtonEvent();

		/*
			Callback event called when the window is being resized
		*/
		void onResizeWindowEvent(const int width, const int height);

		/*
			Callback event called when the window is being moved
		*/
		void onMoveWindowEvent(const int x, const int y);

		/*
			Callback event called on mouse move event
		*/
		void onMouseMotionEvent(const int x, const int y);

		/*
			Callback event called on mouse button event
		*/
		void onMouseButtonEvent(const int button, const kengine::MOUSE_ACTION& state, const int x, const int y);

		/*
			Callback event called on keyboard key event
		*/
		void onKeyboardEvent(const unsigned long long key, const int state);

		/*
			Callback event called on special keyboard keys event like shift, ctrl, etc
		*/
		void onKeyboardSpecialEvent(const unsigned long long key, const int state);

		/*
			Callback event for debug message
		*/
		void debugMessage(const std::string& msg);

		/*
			Custom message pump handler
		*/
		void customWindowProcedure(void* param1, void* param2, void* param3, void* param4);

	private:
		kengine::core* m_engine = nullptr;
		kengine::window* m_window = nullptr;

		kengine::rendering_system* m_renderingSystem = nullptr;
		kengine::GLSLprogram m_shader;
		kengine::render_node logo;
		kengine::texture logoTexture;

		kengine::camera m_cam;

		kengine::timer m_timer;
		float animationTimer = 0.0f;
		kengine::circle_points m_circlePoints;
	};
}

#endif