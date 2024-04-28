/*
	Operating System API Wrapper
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

#ifndef K_ENGINE_OS_API_WRAPPER_HPP
#define K_ENGINE_OS_API_WRAPPER_HPP

#include <events_callback.hpp>

#include <cstdint>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#ifdef _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
#endif

/*
	C++ program entry point
	This global function must be defined by the user's main() function.
*/
int main();

namespace kengine
{
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
		OS initialization
		This function must be called only one time in the application life
	*/
	int osInitialize();

	/*
		OS finish
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
		Set global user events callback
	*/
	void setGlobalUserEventsCallback(events_callback* evt);

	/*
		Message events handling (message pump)
	*/
	void handleSystemMessages();

	/*
		window styles
			- splash (no caption, no resize, no move)
			- default (caption, sys menu, minimize box, maximize box, resize, can move)
			- minimal (caption, sys menu, no resize, can move)
			- minimal no sys menu (caption, no resize, can move)
	*/
	enum class WINDOW_STYLE
	{
		SPLASH,
		MINIMAL,
		MINIMAL_NO_SYS,
		WINDOWED_FULLSCREEN,
		DEFAULT
	};

	enum WINDOW_ADDITIONAL_STYLE
	{
#ifdef _WIN32
		ACCEPT_FILES = WS_EX_ACCEPTFILES,
		APPWINDOW = WS_EX_APPWINDOW,
		COMPOSITED = WS_EX_COMPOSITED,
		NOACTIVATE = WS_EX_NOACTIVATE,
		TOOLWINDOW = WS_EX_TOOLWINDOW,
		TOPMOST = WS_EX_TOPMOST
#else
		ACCEPT_FILES,
		APPWINDOW,
		COMPOSITED,
		NOACTIVATE,
		TOOLWINDOW,
		TOPMOST
#endif
	};

	enum WINDOW_SHOW_TYPE
	{
#ifdef _WIN32
		SHOW = SW_SHOW,
		HIDE = SW_HIDE
#else
		SHOW,
		HIDE
#endif
	};

	/*
		OS Window Wrapper
	*/
	class os_window
	{
	public:
		bool create(int x, int y, int width, int height, std::string name, WINDOW_STYLE style, unsigned long additional = WINDOW_ADDITIONAL_STYLE::APPWINDOW);
		int destroy();
		int show(int showType) const;
		void setWindowText(std::string text);

		int swapBuffers() const;

	private:
#ifdef _WIN32
		HWND hWindow = nullptr;
		HDC hDC = nullptr;
		//HGLRC hRC;
#endif

#ifdef __linux__
		Window hWindow;
#endif
	};
}

#endif