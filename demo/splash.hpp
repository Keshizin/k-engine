/*
	K-Engine Demonstration
	This file provide a splash screen example created with K-Engine.

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

#ifndef K_ENGINE_DEMO_SPLASH_HPP
#define K_ENGINE_DEMO_SPLASH_HPP

#include <core.hpp>
#include <rendering_system.hpp>
#include <camera.hpp>

#include <memory>

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
			Callback event called when the user clicks the window's close button
		*/
		void closeButtonEvent();

		/*
			Callback event called when the window is being resized
		*/
		void onResizeWindowEvent(const int width, const int height);

		/*
			Callback event called on keyboard key event
		*/
		void onKeyboardEvent(const unsigned long long key, const int state);

		/*
			Callback event for debug message
		*/
		void debugMessage(const std::string& msg);

		HIDE_ENGINE_EVENT_CALLBACK()
		HIDE_MOUSE_EVENT_CALLBACK()
		HIDE_SPECIAL_KEYBOARD_EVENT_CALLBACK()
		HIDE_CUSTOM_WINPROC_EVENT_CALLBACK()

	private:
		kengine::core* m_engine = nullptr;
		std::unique_ptr<kengine::window> m_window;
		std::unique_ptr<kengine::rendering_system> m_renderingSystem;
		std::unique_ptr<kengine::primitive_render> m_primitive;

		kengine::GLSLprogram m_shader;
		kengine::gpu_buffer m_gpuBuffer;
		kengine::texture m_texture;
		kengine::camera m_cam;
		kengine::timer m_sceneTimer;
	};
}

#endif