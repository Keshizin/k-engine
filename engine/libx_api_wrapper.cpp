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

#ifndef __ANDROID__
#ifdef __linux__

// k-engine headers
#include <os_api_wrapper.hpp>
#include <k_version.hpp>
// std headers
#include <cassert>
#include <iostream>
// linux headers
#include <time.h>


/*
	Global Objects
*/

#define LINUX_TIME_RESOLUTION 1000000000L

static kengine::events_callback* globalUserEventsCallback = nullptr;


/*
	LIBX API Global Objects
*/

int xWindowProcedure(const XEvent& event);


/*
	OpenGL Procedures Extension for LibX (GLX)
*/

typedef GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;


/*
 ****************************************************************************
  K-Engine public interface for OS API functions
 ****************************************************************************
*/

namespace kengine
{
	/*
	*/
	class xlib_global_app_manager : public global_app_manager
	{
	public:
		xlib_global_app_manager() {
			m_display = XOpenDisplay(nullptr);

			if (m_display == nullptr)
			{
				assert(!(globalUserEventsCallback == nullptr));
				globalUserEventsCallback->debugMessage("It was not possible to connect to X server");
			}
		};

		~xlib_global_app_manager() {
			XCloseDisplay(m_display);
		};

		xlib_global_app_manager(const xlib_global_app_manager& copy) = delete; // copy constructor
		xlib_global_app_manager& operator=(const xlib_global_app_manager& copy) = delete; // copy assignment
		xlib_global_app_manager(xlib_global_app_manager&& move) noexcept = delete;  // move constructor
		xlib_global_app_manager& operator=(xlib_global_app_manager&&) = delete; // move assigment

		Display* m_display = nullptr;
		unsigned int m_event_mask = StructureNotifyMask | FocusChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	};

	static xlib_global_app_manager* globalAppManager = nullptr;

	/*
	*/
	class xlib_window : public window
	{
		friend class xlib_rendering_context;

	public:
		xlib_window() {}

		~xlib_window() {
			if (hWindow)
				destroy();
		}

		xlib_window(const xlib_window& copy) = delete; // copy constructor
		xlib_window& operator=(const xlib_window& copy) = delete; // copy assignment
		xlib_window(xlib_window&& move) noexcept = delete;  // move constructor
		xlib_window& operator=(xlib_window&&) = delete; // move assigment

		bool create(int xPos, int yPos, int widthParam, int heightParam, const std::string& nameParam) {
			x = xPos;
			y = yPos;
			width = widthParam;
			height = heightParam;
			name = nameParam;

			XSetWindowAttributes attributes;
			attributes.event_mask = globalAppManager->m_event_mask;

			hWindow = XCreateWindow(
				globalAppManager->m_display,
				DefaultRootWindow(globalAppManager->m_display), // or RootWindowOfScreen(screen)
				x,
				y,
				width,
				height,
				0,
				CopyFromParent, // depth
				InputOutput,
				CopyFromParent, // visual,
				CWEventMask,
				&attributes);

			XStoreName(globalAppManager->m_display, hWindow, name.c_str());

			Atom wm_delete = XInternAtom(globalAppManager->m_display, "WM_DELETE_WINDOW", False);
			XSetWMProtocols(globalAppManager->m_display, hWindow, &wm_delete, 1);

			if (globalUserEventsCallback && globalAppManager->isCallbackCallsEnabled())
				globalUserEventsCallback->onWindowReady(this);

			return true;
		}

		bool createGLX() {
			GLint majorVersion = 0;
			GLint minorVersion = 0;
			int screenNumber = DefaultScreen(globalAppManager->m_display);

			glXQueryVersion(globalAppManager->m_display, &majorVersion, &minorVersion);
			globalUserEventsCallback->debugMessage("> GLX version: " + std::to_string(majorVersion) + std::to_string(minorVersion));
			globalUserEventsCallback->debugMessage("> GLX client version: " + std::string(glXGetClientString(globalAppManager->m_display, GLX_VERSION)));
			globalUserEventsCallback->debugMessage("> GLX client vendor: " + std::string(glXGetClientString(globalAppManager->m_display, GLX_VENDOR)));
			//globalUserEventsCallback->debugMessage("> GLX client extensions: " + std::string(glXGetClientString(globalAppManager->m_display, GLX_EXTENSIONS)));
			globalUserEventsCallback->debugMessage("> GLX server version: " + std::string(glXQueryServerString(globalAppManager->m_display, screenNumber, GLX_VERSION)));
			globalUserEventsCallback->debugMessage("> GLX server vendor: " + std::string(glXQueryServerString(globalAppManager->m_display, screenNumber, GLX_VENDOR)));
			//globalUserEventsCallback->debugMessage("> GLX server extensions: " + std::string(glXQueryServerString(globalAppManager->m_display, screenNumber, GLX_EXTENSIONS)));

			GLint glxAttributes[] = {
				GLX_X_RENDERABLE, True,
				GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
				GLX_RED_SIZE, 8,
				GLX_GREEN_SIZE, 8,
				GLX_BLUE_SIZE, 8,
				GLX_ALPHA_SIZE, 8,
				GLX_DEPTH_SIZE, 24,
				GLX_STENCIL_SIZE, 8,
				GLX_DOUBLEBUFFER, True,
				None
			};

			int framebufferCounter = 0;
			GLXFBConfig* frameBufferConfigList = glXChooseFBConfig(globalAppManager->m_display, screenNumber, glxAttributes, &framebufferCounter);

			if (frameBufferConfigList == nullptr)
			{
				globalUserEventsCallback->debugMessage("Failed to retrieve a list of GLX framebuffer configurations");
				return false;
			}

			//globalUserEventsCallback->debugMessage("> framebuffer counter: " + std::to_string(framebufferCounter));

			int bestFramebufferConfigIndex = -1;
			int bestSample = -1;

			for (int i = 0; i < framebufferCounter; ++i)
			{
				int sampleBuffers, samples;
				glXGetFBConfigAttrib(globalAppManager->m_display, frameBufferConfigList[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
				glXGetFBConfigAttrib(globalAppManager->m_display, frameBufferConfigList[i], GLX_SAMPLES, &samples);

				//globalUserEventsCallback->debugMessage("> GLX framebuffer [" + std::to_string(i) + "] sample buffers : " + std::to_string(sampleBuffers));
				//globalUserEventsCallback->debugMessage("> GLX framebuffer [" + std::to_string(i) + "] samples: " + std::to_string(samples));

				if (bestFramebufferConfigIndex < 0 || (sampleBuffers && samples > bestSample)) {
					bestFramebufferConfigIndex = i;
					bestSample = samples;
				}
			}

			framebufferConfig = frameBufferConfigList[bestFramebufferConfigIndex];

			//globalUserEventsCallback->debugMessage("> best GLX framebuffer config: " + std::to_string(bestFramebufferConfigIndex));

			visualInfo = glXGetVisualFromFBConfig(globalAppManager->m_display, framebufferConfig);
			XFree(frameBufferConfigList);

			if (visualInfo == nullptr) {
				std::cout << "Failed to get GLX visual from framebuffer config.\n";
				return false;
			}

			XSetWindowAttributes attributes;
			//attributes.border_pixel = BlackPixel(globalAppManager->m_display, screenNumber);
			//attributes.background_pixel = WhitePixel(display, screenId);
			attributes.override_redirect = True;
			cmap = XCreateColormap(globalAppManager->m_display, DefaultRootWindow(globalAppManager->m_display), visualInfo->visual, AllocNone);
			attributes.colormap = cmap;
			attributes.event_mask = globalAppManager->m_event_mask;

			hWindow = XCreateWindow(
				globalAppManager->m_display,
				DefaultRootWindow(globalAppManager->m_display), // or RootWindowOfScreen(screen)
				x,
				y,
				width,
				height,
				0,
				visualInfo->depth, // depth
				InputOutput,
				visualInfo->visual, // visual,
				CWColormap | CWEventMask, // CWBorderPixel, CWBackPixel
				&attributes);

			Atom windowMessageDelete = XInternAtom(globalAppManager->m_display, "WM_DELETE_WINDOW", False);
			XSetWMProtocols(globalAppManager->m_display, hWindow, &windowMessageDelete, 1);

			//if (globalUserEventsCallback && globalAppManager->isCallbackCallsEnabled())
			//	globalUserEventsCallback->onWindowReady(this);

			return true;
		}

		int destroy() {
			if (visualInfo != nullptr)
			{
				XFreeColormap(globalAppManager->m_display, cmap);

				XFree(visualInfo);
				visualInfo = nullptr;
			}

			XDestroyWindow(globalAppManager->m_display, hWindow);

			hWindow = 0;
			return 1;
		}

		int show() {
			XMapWindow(globalAppManager->m_display, hWindow);
			return 1;
		}

		int hide() {
			XUnmapWindow(globalAppManager->m_display, hWindow);
			return 1;
		}

		int swapBuffers()  const {
			if(hWindow)
				glXSwapBuffers(globalAppManager->m_display, hWindow);
			return 1;
		}

		void setWindowText(std::string text)
		{
			XStoreName(globalAppManager->m_display, hWindow, text.c_str());
		}

		void flushEvents() const
		{
			XSync(globalAppManager->m_display, true);
		}

	private:
		Window hWindow = 0;
		XVisualInfo* visualInfo = nullptr;
		Colormap cmap;
		GLXFBConfig framebufferConfig;
	};

	window* windowInstance()
	{
		return new xlib_window;
	}


	/*
		kengine::xlib_rendering_context - class members definition
	*/
	class xlib_rendering_context : public rendering_context
	{
	public:
		explicit xlib_rendering_context(window* windowParam) {
			assert(!(windowParam == nullptr));
			xlib_win = dynamic_cast<xlib_window*>(windowParam);
		}

		~xlib_rendering_context() {
			if (hRC != nullptr)
				destroy();
		}

		xlib_rendering_context(const xlib_rendering_context& copy) = delete; // copy constructor
		xlib_rendering_context& operator=(const xlib_rendering_context& copy) = delete; // copy assignment
		xlib_rendering_context(xlib_rendering_context&& move) noexcept = delete;  // move constructor
		xlib_rendering_context& operator=(xlib_rendering_context&&) = delete; // move assigment

		int create() {
#ifndef GLX_ARB_create_context 
			globalUserEventsCallback->debugMessage("The GLX_ARB_create_context extension is not defined");
			return 0;
#endif

#ifndef GLX_ARB_create_context_profile
			globalUserEventsCallback->debugMessage("The GLX_ARB_create_context_profile extension is not defined");
			return 0;
#endif

			globalAppManager->setCallbackCalls(false);

			// destroy the window to create a GLX window
			if (!xlib_win->destroy())
				return 0;

			// flush the queue events
			xlib_win->flushEvents(); 

			// create a new GLX window
			if (!xlib_win->createGLX())
				return 0;

			if (!kengine::getAllGLProcedureAddress())
				return 0;

			//hRC = glXCreateContext(globalAppManager->m_display, win->visualInfo, nullptr, GL_TRUE); // legacy

			int attribList[] = {
			   GLX_CONTEXT_MAJOR_VERSION_ARB, KENGINE_OPENGL_MAJOR_VERSION,
			   GLX_CONTEXT_MINOR_VERSION_ARB, KENGINE_OPENGL_MINOR_VERSION,
			   //GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			   GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			   None
			};

			hRC = glXCreateContextAttribsARB(globalAppManager->m_display, xlib_win->framebufferConfig, 0, true, attribList);
			XSync(globalAppManager->m_display, False);

			makeCurrent(true);
			globalAppManager->setCallbackCalls(true);

			return 1;
		}

		int destroy() {
			makeCurrent(false);
			glXDestroyContext(globalAppManager->m_display, hRC);
			return 1;
		}

		int makeCurrent(bool enable) {
			if (enable)
			{
				glXMakeCurrent(globalAppManager->m_display, xlib_win->hWindow, hRC);
			}
			else
			{
				glXMakeCurrent(globalAppManager->m_display, None, NULL);
			}

			return 1;
		}

		int swapBuffers()
		{
			return 1;
		}

	private:
		xlib_window* xlib_win = nullptr; // avoid dynamic casts in the methods
		GLXContext hRC;
	};

	rendering_context* renderingContextInstance(window* win)
	{
		return new xlib_rendering_context(win);
	}
}

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


int kengine::createDebugConsole()
{
	return 1;
}


int kengine::closeDebugConsole()
{
	return 1;
}


int kengine::osInitialize()
{
	if (globalAppManager != nullptr)
		delete globalAppManager;

	globalAppManager = new xlib_global_app_manager;

	return 1;
}


int kengine::osFinish()
{
	delete globalAppManager;
	return 1;
}


void kengine::getDisplaySize(int* width, int* height)
{
	assert(!(globalAppManager == nullptr));
	int screenNumber = DefaultScreen(globalAppManager->m_display);
	*width = DisplayWidth(globalAppManager->m_display, screenNumber);
	*height = DisplayHeight(globalAppManager->m_display, screenNumber);
}


int kengine::getDisplayCenterPosX(int width)
{
	assert(!(globalAppManager == nullptr));
	int screenNumber = DefaultScreen(globalAppManager->m_display);
	return (DisplayWidth(globalAppManager->m_display, screenNumber) - width) / 2;
}


int kengine::getDisplayCenterPosY(int height)
{
	assert(!(globalAppManager == nullptr));
	int screenNumber = DefaultScreen(globalAppManager->m_display);
	return (DisplayHeight(globalAppManager->m_display, screenNumber) - height) / 2;
}


int kengine::getNumberOfMonitors()
{
	assert(!(globalAppManager == nullptr));
	return ScreenCount(globalAppManager->m_display);
}


void kengine::quitApplication(int returnCode)
{
	assert(!(globalUserEventsCallback == nullptr));
	globalUserEventsCallback->onFinishEvent();

}


void kengine::setGlobalUserEventsCallback(events_callback* evt)
{
	assert(globalUserEventsCallback == nullptr); // trying to override a globalUserEventsCallback
	globalUserEventsCallback = evt;
}


void kengine::handleSystemMessages()
{
	// (!) PLEASE DON'T INCLUDE I/O's STUFF HERE!
	XEvent xEvent;

	// This event message is like WM_CLOSE in Win32
	if(XCheckTypedEvent(globalAppManager->m_display, ClientMessage, &xEvent))
	{
		globalUserEventsCallback->closeButtonEvent();
	}

	if(XCheckMaskEvent(globalAppManager->m_display, globalAppManager->m_event_mask, &xEvent))
		xWindowProcedure(xEvent);
}


void* kengine::getGLFunctionAddress(std::string name)
{
	/*
		References:
			- https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
	*/

	//return glXGetProcAddressARB(name.c_str());
	return nullptr;
}


int kengine::getAllGLProcedureAddress()
{
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

	if (glXCreateContextAttribsARB == nullptr)
	{
		return 0;
	}

	return 1;
}


int xWindowProcedure(const XEvent& event)
{
	assert(!(globalUserEventsCallback == nullptr));

	if (!kengine::globalAppManager->isCallbackCallsEnabled())
		return 1;
	
	switch(event.type)
	{
	/*
		Window state notification events
	*/
	case ConfigureNotify: // WM_MOVE, WM_SIZE
		globalUserEventsCallback->onMoveWindowEvent(event.xconfigure.x, event.xconfigure.y);
		globalUserEventsCallback->onResizeWindowEvent(event.xconfigure.width, event.xconfigure.height);
		break;

	case DestroyNotify: // WM_DESTROY
		globalUserEventsCallback->onWindowDestroy();
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
#endif