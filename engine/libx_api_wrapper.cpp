/*
	LibX API Wrapper
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

#ifdef __linux__

#include <os_api_wrapper.hpp>

#include <iostream>
#include <cassert>

#include <time.h>
#define LINUX_TIME_RESOLUTION 1000000000L

static kengine::events_callback* globalUserEventsCallback = nullptr;
static Display* g_display = nullptr;

static unsigned int g_event_mask =
	StructureNotifyMask |
	FocusChangeMask |
	KeyPressMask | KeyReleaseMask |
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

int xWindowProcedure(const XEvent& event);

int64_t kengine::getHighResolutionTimerCounter()
{
	timespec t;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	return (t.tv_sec * LINUX_TIME_RESOLUTION) + t.tv_nsec;
}

int64_t kengine::getHighResolutionTimerFrequency()
{
	timespec t;
	clock_getres(CLOCK_PROCESS_CPUTIME_ID, &t);
	return t.tv_nsec * LINUX_TIME_RESOLUTION;
}

/*
	Creating a debug console
*/
int kengine::createDebugConsole()
{
	return 1;
}

/*
	Closing the debug console
*/
int kengine::closeDebugConsole()
{
	return 1;
}

/*
	OS initialization
	This function must be called only one time
*/
int kengine::osInitialize()
{
	g_display = XOpenDisplay(nullptr);

	if(g_display == nullptr)
	{
		assert(!(globalUserEventsCallback == nullptr));
		globalUserEventsCallback->debugMessage("It was not possible to connect to X server");
		return 0;
	}

	return 1;
}

/*
	OS finish
*/
int kengine::osFinish()
{
	XCloseDisplay(g_display);
	g_display = nullptr;
	return 1;
}

/*
	Get the primary display size in pixels
*/
void kengine::getDisplaySize(int* width, int* height)
{
	assert(!(g_display == nullptr));
	int screenNumber = DefaultScreen(g_display);
	*width = DisplayWidth(g_display, screenNumber);
	*height = DisplayHeight(g_display, screenNumber);
}

/*
	Get the center position (x axis) of the primary display
*/
int kengine::getDisplayCenterPosX(int width)
{
	assert(!(g_display == nullptr));
	int screenNumber = DefaultScreen(g_display);
	return (DisplayWidth(g_display, screenNumber) - width) / 2;
}

/*
	Get the center position in y axis of the primary display
*/
int kengine::getDisplayCenterPosY(int height)
{
	assert(!(g_display == nullptr));
	int screenNumber = DefaultScreen(g_display);
	return (DisplayHeight(g_display, screenNumber) - height) / 2;
}

/*
	Get the number of monitors
*/
int kengine::getNumberOfMonitors()
{
	assert(!(g_display == nullptr));
	return ScreenCount(g_display);
}

/*
	Set global callback events
*/
void kengine::setGlobalUserEventsCallback(events_callback* evt)
{
	assert(globalUserEventsCallback == nullptr); // trying to override a globalUserEventsCallback
	globalUserEventsCallback = evt;
}

/*
	Message events handling (message pump)
*/
void kengine::handleSystemMessages()
{
	// (!) PLEASE DON'T INCLUDE I/O's STUFF HERE!
	XEvent xEvent;

	// This event message is like WM_CLOSE in Win32
	if(XCheckTypedEvent(g_display, ClientMessage, &xEvent))
	{
		globalUserEventsCallback->beforeFinishEvent();
	}

	if(XCheckMaskEvent(g_display, g_event_mask, &xEvent))
		xWindowProcedure(xEvent);
}

/*
	kengine::os_window class member definition
*/

bool kengine::os_window::create(int x, int y, int width, int height, std::string name, WINDOW_STYLE style, unsigned long additional)
{
	//GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	//XVisualInfo* vi = glXChooseVisual(g_display, 0, att);
	//auto cmap = XCreateColormap(g_display, root, vi->visual, AllocNone);

	XSetWindowAttributes attributes;
	//attributes.colormap = cmap;
	attributes.event_mask = g_event_mask;
	
	hWindow = XCreateWindow(
		g_display,
		DefaultRootWindow(g_display),
		x,
		y,
		width,
		height,
		0,
		CopyFromParent, // vi->depth,
		InputOutput,
		CopyFromParent, // vi->visual,
		CWEventMask, // CWColormap | CWEventMask,
		&attributes);

	XStoreName(g_display, hWindow, name.c_str());

	Atom wm_delete = XInternAtom(g_display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(g_display, hWindow, &wm_delete, 1);

	// auto myGLContext = glXCreateContext(g_display, vi, nullptr, 1);
	// glXMakeCurrent(g_display, hWindow, myGLContext);

	return true;
}

int kengine::os_window::destroy()
{
	XDestroyWindow(g_display, hWindow);
	hWindow = 0;
	return 1;
}

int kengine::os_window::show(int showType) const
{
	XMapWindow(g_display, hWindow);
	return 1;
}

void kengine::os_window::setWindowText(std::string text)
{
	XStoreName(g_display, hWindow, text.c_str());
}

int kengine::os_window::swapBuffers() const
{
	//glXSwapBuffers(g_display, hWindow);
	return 1;
}

/*
*/
int xWindowProcedure(const XEvent& event)
{
	assert(!(globalUserEventsCallback == nullptr));

	switch(event.type)
	{
	/*
		Window state notification events
	*/
	case ConfigureNotify: // WM_MOVE, WM_SIZE
		globalUserEventsCallback->onMoveWindowEvent(event.xconfigure.x, event.xconfigure.y);
		globalUserEventsCallback->onResizeWindowEvent(event.xconfigure.width, event.xconfigure.height);
		break;

	case DestroyNotify: // WM_QUIT
		globalUserEventsCallback->afterFinishEvent();
		return 0;

	case FocusIn: // WM_ACTIVATE
		globalUserEventsCallback->onResumeEvent();
		break;

	case FocusOut: // WM_INACTIVATE
		globalUserEventsCallback->onPauseEvent();
		break;

	/*
		Keyboard events
	*/
	case KeyPress:
		globalUserEventsCallback->onKeyboardEvent(event.xkey.keycode, 1);
		globalUserEventsCallback->onKeyboardSpecialEvent(event.xkey.keycode, 1);
		break;

	case KeyRelease:
		globalUserEventsCallback->onKeyboardEvent(event.xkey.keycode, 0);
		globalUserEventsCallback->onKeyboardSpecialEvent(event.xkey.keycode, 0);
		break;

	/*
		Pointer events
	*/
	case ButtonPress:
		globalUserEventsCallback->onMouseButtonEvent(event.xbutton.button, kengine::MOUSE_ACTION::DOWN, event.xbutton.x, event.xbutton.y);
		break;

	case ButtonRelease:
		globalUserEventsCallback->onMouseButtonEvent(event.xbutton.button, kengine::MOUSE_ACTION::UP, event.xbutton.x, event.xbutton.y);
		break;

	case MotionNotify: // WM_MOUSEMOVE
		globalUserEventsCallback->onMouseMotionEvent(event.xmotion.x, event.xmotion.y);
		break;

	default:
		break;
	}

	return 1;
}

#endif