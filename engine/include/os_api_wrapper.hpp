/*
	Operating System API Wrapper
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
		Enum for keys
		(!) TODO: criar um header especifico para isso para não aumentar o tamanho deste header
	*/
	enum KEY : unsigned long long
	{
#if defined(_WIN32)
		ENTER = VK_RETURN,
		ESCAPE = VK_ESCAPE,
		TAB = VK_TAB,
		OEM_3 = VK_OEM_3

#elif(__ANDROID__)
		ENTER,
		ESCAPE
#elif(__linux__)
		ENTER,
		ESCAPE
#endif
	};

	/*
		OS Window style
	*/
	enum class window_style
	{
		SPLASH, // no caption, no resize, no move
		MINIMAL, // caption, sys menu, no resize, can move
		MINIMAL_NO_SYS, // caption, no resize, can move
		WINDOWED_FULLSCREEN,
		DEFAULT // caption, sys menu, minimize box, maximize box, resize, can move
	};

	/*
		This class is an abstract class for the application window.
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

		virtual bool create(int x, int y, int width, int height, const std::string& name, const window_style& style) = 0;
		virtual int destroy() = 0;
		virtual int show() = 0;
		virtual int hide() = 0;
		virtual int swapBuffers() const = 0;
		virtual void* getHandle() = 0;

		void setWindowSize(int w, int h) { width = w; height = h; }
		int getWidth() const { return width; }
		int getHeight() const { return height; }

	protected:
		std::string name; // window bar title
		int x = 0; // position in X axis (in pixels units)
		int y = 0; // position in Y axis (in pixels units)
		int width = 640; // window width in pixels size
		int height = 480; // window height in pixels size
		window_style m_style = window_style::DEFAULT;
	};

	/*
		Returns an instance based on the platform (Win32, LibX ou Android) window
	*/
	std::unique_ptr<kengine::window> windowInstance();

	/*
	*/
	class rendering_context_info
	{
	public:
		int major = 0;
		int minor = 0;
	};

	/*
		kengine::rendering_context is an abstract class for graphis API context e.g. OpenGL.
	*/
	class rendering_context
	{
	public:
		rendering_context() {}
		virtual ~rendering_context() {}

		rendering_context(const rendering_context& copy) = delete; // copy constructor
		rendering_context& operator=(const rendering_context& copy) = delete; // copy assignment
		rendering_context(rendering_context&& move) noexcept = delete;  // move constructor
		rendering_context& operator=(rendering_context&&) = delete; // move assigment

		virtual int create(const compatibility_profile& profile) = 0;
		virtual int destroy() = 0;
		virtual void makeCurrent() = 0;
		virtual void unmakeCurrent() = 0;
		virtual int swapBuffers() = 0;
	
		void setRenderingContextInfo(int major, int minor) { info.major = major; info.minor = minor; }

	private:
		rendering_context_info info;
	};


	/*
		Returns an instance based on the plaform (Win32, LibX ou Android)
	*/
	std::unique_ptr<rendering_context> renderingContextInstance(window* windowParam);


	/*
		Global App Manager Class
			- allows to controle the callback events calls
			- allows to manage the global variables of the platfom (LibX and Android)
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

	/*
		This method specify the swap interval (synchronization of the front and back frame buffers with vertical blanks)
		@param value 1 enable vsync and value 0 disable vsync
	*/
	int setVSync(int vsync);

	/*
		Get the path of executable directoty
	*/
	std::string getExePath();
}

#endif