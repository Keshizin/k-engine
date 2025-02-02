/*
	Operating System API Wrapper
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

#ifndef K_ENGINE_OS_API_WRAPPER_HPP
#define K_ENGINE_OS_API_WRAPPER_HPP

// k-engine headers
#include <events_callback.hpp>
#include <gl_wrapper.hpp>
// std headers
#include <cstdint>
#include <string>
// os headers
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__ANDROID__)
#include <android/log.h>
#include <android_native_app_glue.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif

/*
	K-Engine public interface for OS API functions
*/
namespace kengine
{
	/*
		This class is an abstract class for the application window
	*/
	class window
	{
	public:
		window() {}
		virtual ~window() {}

		window(const window& copy) = delete; // copy constructor
		window& operator=(const window& copy) = delete; // copy assignment
		window(window&& move) noexcept = delete;  // move constructor
		window& operator=(window&&) = delete; // move assigment

		virtual bool create(int x, int y, int width, int height, const std::string& name) = 0;
		virtual int destroy() = 0;
		virtual int show() = 0;
		virtual int hide() = 0;
		virtual int swapBuffers() const = 0;

		virtual void* getHandle() = 0;

	protected:
		std::string name; // window bar title
		int x = 0; // position in X axis (in pixels units)
		int y = 0; // position in Y axis (in pixels units)
		int width = 640; // window width in pixels size
		int height = 480; // window height in pixels size
	};

	/*
		Retorna uma instância baseada na plataforma (Win32, LibX ou Android)
	*/
	window* windowInstance();

	/*
	*/
	class rendering_context_info
	{
	public:
		int major;
		int minor;
	};

	/*
		kengine::rendering_context is an abstract class for graphis API context e.g. OpenGL.
	*/
	class rendering_context
	{
	public:
		virtual ~rendering_context() {}

		virtual int create(const compatibility_profile& profile) = 0;
		virtual int destroy() = 0;
		virtual int makeCurrent(bool enable) = 0;
		virtual int swapBuffers() = 0;
		virtual void clearBuffers() = 0;

		void setRenderingContextInfo(int major, int minor) { info.major = major; info.minor = minor; }

	private:
		rendering_context_info info;
	};

	/*
		Retorna uma instância baseada na plataforma (Win32, LibX ou Android)
	*/
	rendering_context* renderingContextInstance(window* windowParam);


	/*
		Global App Manager Class
			- permite controlar as chamadas de funções de callback
			- permite gerenciar váriaveis globais específicos de plataformas com LibX e Android
	*/
	class global_app_manager
	{
	public:
		global_app_manager() {};
		~global_app_manager() {};

		global_app_manager(const global_app_manager& copy) = delete; // copy constructor
		global_app_manager& operator=(const global_app_manager& copy) = delete; // copy assignment
		global_app_manager(global_app_manager&& move) noexcept = delete;  // move constructor
		global_app_manager& operator=(global_app_manager&&) = delete; // move assigment

		void setCallbackCalls(bool enable) { m_callback_calls_enable = enable; }
		bool isCallbackCallsEnabled() const { return m_callback_calls_enable; }

	private:
		bool m_callback_calls_enable = true;
	};

	/*
		Retrieves the current value of the perfomance counter, which is a high resolution (<1us) time stamp that can be used for time-interval measurements.
	*/
	int64_t getHighResolutionTimerCounter();

	/*
		Retrieves the frequency of the perfomance counter. The frequency of the perfomance counter is fixed at system boot and is consistent across all processors. Therefore, the frequency need only be queried upon application initialization, and the result can be cached.
	*/
	int64_t getHighResolutionTimerFrequency();

	/*
		Creating a debug console
	*/
	int createDebugConsole();

	/*
		Closing the debug console
	*/
	int closeDebugConsole();

	/*
		OSInitialize is used to initialize global objects from OS API (e.g. Win32, XLib and Android NDK)
		This function must be called only one time in the application life
	*/
	int osInitialize();

	/*
		OSFinish is used to finish and release the global objects from OS API (e.g. Win32, XLib and Android NDK)
		This function must be called only one time in the application life

		Nota: Esta função deve ser obrigatoriamente usada como RAII
	*/
	int osFinish();

	/*
		Get the primary display size in pixels
	*/
	void getDisplaySize(int* width, int* height);

	/*
		Get the center position in x axis of the primary display
	*/
	int getDisplayCenterPosX(int displayWidth);

	/*
		Get the center position in y axis of the primary display
	*/
	int getDisplayCenterPosY(int displayHeight);

	/*
		Get the number of monitors
	*/
	int getNumberOfMonitors();

	/*
		Get DPI (dots per inch) value for the specified window
	*/
	//unsigned int getDPI(const window* win);

	/*
		Send QUIT message to indicate to the system that a thread has made a request to terminate (quit).
	*/
	void quitApplication(int returnCode);

	/*
		Set global user events callback
	*/
	void setGlobalUserEventsCallback(events_callback* evt);

	/*
		Message events handling (message pump)
	*/
	void handleSystemMessages();

	/*
		Getting OpenGL functions and OS extension functions
	*/
	int getAllGLProcedureAddress();
}

#endif