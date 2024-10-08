/*
    Android API Wrapper
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

#if defined(__ANDROID__)

// k-engine headers
#include <os_api_wrapper.hpp>
#include <k_version.hpp>
// std headers
#include <cassert>
// android/linux headers
#include <time.h>


/*
	Global Objects
*/

#define LINUX_TIME_RESOLUTION 1000000000L

static kengine::events_callback* globalUserEventsCallback = nullptr;


/*
	Android NDK Global Objects
*/

void androidHandleCmd(struct android_app* app, int32_t cmd);
int32_t androidHandleInput(android_app* app, AInputEvent* event);


/*
    OpenGL Procedures Extension for Android
*/

// IMPORTGL_API EGLBoolean (*funcPtr_eglSwapBuffers)(EGLDisplay dpy, EGLSurface draw) IMPORTGL_FNPTRINIT

/*
 ****************************************************************************
  K-Engine public interface for OS API functions
 ****************************************************************************
*/

namespace kengine
{
    class android_NativeWindow : public window
    {
        friend class android_rendering_context;
    public:
        android_NativeWindow() {}
        ~android_NativeWindow() {}

        android_NativeWindow(const android_NativeWindow& copy) = delete; // copy constructor
        android_NativeWindow& operator=(const android_NativeWindow& copy) = delete; // copy assignment
        android_NativeWindow(android_NativeWindow&& move) noexcept = delete;  // move constructor
        android_NativeWindow& operator=(android_NativeWindow&&) = delete; // move assigment

        bool create(int x, int y, int width, int height, const std::string& name) { return 1; }
        int destroy() { return 1; }
        int show() { return 1; }
        int hide() { return 1; }
        int swapBuffers() const { return 1; }

        ANativeWindow* m_hWindow = nullptr;
    };

    window* windowInstance() {
        return new android_NativeWindow;
    }

    class android_rendering_context : public rendering_context
    {
    public:
        explicit android_rendering_context(window* windowParam)
        {
            win = dynamic_cast<android_NativeWindow*>(windowParam);
        }

        ~android_rendering_context() { destroy(); }

        int create()
        {
            eglDisplayID = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            if (eglDisplayID == EGL_NO_DISPLAY)
            {
                K_LOG_OUTPUT_RAW("Failed to get EGL Display");
                return 0;
            }

            int major;
            int minor;

            eglInitialize(eglDisplayID, &major, &minor);
            setRenderingContextInfo(major, minor);

            std::string eglMajorMSG = "EGL major: " + std::to_string(major);
            K_LOG_OUTPUT_RAW(eglMajorMSG.c_str());
            std::string eglMinorMSG = "EGL minor: " + std::to_string(minor);
            K_LOG_OUTPUT_RAW(eglMinorMSG.c_str());

            const EGLint attribs[] = {
                    EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
                    EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                    EGL_ALPHA_SIZE, 8,
                    EGL_BLUE_SIZE,8,
                    EGL_GREEN_SIZE,8,
                    EGL_RED_SIZE,8,
                    EGL_DEPTH_SIZE,24,
                    EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs;

            //eglChooseConfig(eglDisplayID, attribs, NULL, 1, &numConfigs); // return the total number of configs
            eglChooseConfig(eglDisplayID, attribs, &config, 1, &numConfigs);

            if (config == NULL)
            {
                destroy();
                K_LOG_OUTPUT_RAW("Failed to choose EGL config");
                return 0;
            }

            eglSurface = eglCreateWindowSurface(eglDisplayID, config, win->m_hWindow, nullptr);

            if (eglSurface == EGL_NO_SURFACE)
            {
                destroy();
                K_LOG_OUTPUT_RAW("Failed to create window surface");
                return 0;
            }

            eglQuerySurface(eglDisplayID, eglSurface, EGL_WIDTH, &win->width);
            eglQuerySurface(eglDisplayID, eglSurface, EGL_HEIGHT, &win->height);

            std::string width = "EGL WIDTH: " + std::to_string(win->width);
            std::string height = "EGL HEIGHT: " + std::to_string(win->height);
            K_LOG_OUTPUT_RAW(width.c_str());
            K_LOG_OUTPUT_RAW(height.c_str());

            // creating context
            const EGLint context_attribs[] = {
                    EGL_CONTEXT_MAJOR_VERSION, 3,
                    EGL_CONTEXT_MINOR_VERSION, 2,
                    //EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT, // or EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT
                    //EGL_CONTEXT_OPENGL_DEBUG, EGL_FALSE,
                    //EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_FALSE,
                    //EGL_CONTEXT_OPENGL_ROBUST_ACCESS, EGL_FALSE,
                    //EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY,
                    EGL_NONE
            };

            eglContext = eglCreateContext(eglDisplayID, config, nullptr, context_attribs);

            if (eglContext == EGL_NO_CONTEXT)
            {
                destroy();
                K_LOG_OUTPUT_RAW("Failed to create EGL context");
                return 0;
            }

            makeCurrent(true);

            return 1;
        }

        int destroy()
        {
            if (eglContext != EGL_NO_CONTEXT)
            {
                eglDestroyContext(eglDisplayID, eglContext);
                eglContext = EGL_NO_CONTEXT;
            }

            if (eglSurface != EGL_NO_SURFACE)
            {
                eglDestroySurface(eglDisplayID, eglSurface);
                eglSurface = EGL_NO_SURFACE;
            }

            eglTerminate(eglDisplayID);
            eglDisplayID = EGL_NO_DISPLAY;
            return 1;
        }

        int makeCurrent(bool enable)
        {
            if (enable)
                return eglMakeCurrent(eglDisplayID, eglSurface, eglSurface, eglContext);
            else
                return eglMakeCurrent(eglDisplayID, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        }

        int swapBuffers() {
            return eglSwapBuffers(eglDisplayID, eglSurface);
        }

    private:
        EGLDisplay eglDisplayID = EGL_NO_DISPLAY;
        EGLSurface eglSurface = EGL_NO_SURFACE;
        EGLContext eglContext = EGL_NO_CONTEXT;
        android_NativeWindow* win = nullptr;
    };

    rendering_context* renderingContextInstance(window* win) {
        return new android_rendering_context(win);
    }


    class android_global_app_manager : public global_app_manager
    {
    public:
        android_global_app_manager() {};
        ~android_global_app_manager() {};

        android_global_app_manager(const android_global_app_manager& copy) = delete; // copy constructor
        android_global_app_manager& operator=(const android_global_app_manager& copy) = delete; // copy assignment
        android_global_app_manager(android_global_app_manager&& move) noexcept = delete;  // move constructor
        android_global_app_manager& operator=(android_global_app_manager&&) = delete; // move assigment

        // https://developer.android.com/reference/games/game-activity/struct/android-app
        struct android_app* m_android_app = nullptr;
        android_NativeWindow m_android_window;
    };

    static android_global_app_manager globalAppManager;
}


/*
	C++ program entry point
*/
int main();


/*
    Android native application main entry point (android_native_app_glue)
    It runs in its own thread, with its own event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state) {
    kengine::globalAppManager.m_android_app = state;
    main();
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
    //assert(!(globalAppManager.m_android_app == nullptr));
    globalAppManager.m_android_app->onAppCmd = androidHandleCmd;
    globalAppManager.m_android_app->onInputEvent = androidHandleInput;
    return 1;
}

/*
    OS finish
*/
int kengine::osFinish()
{
    return 1;
}

/*
    Get the primary display size in pixels
*/
void kengine::getDisplaySize(int* width, int* height)
{
}

/*
    Get the center position (x axis) of the primary display
*/
int kengine::getDisplayCenterPosX(int width)
{
    return 1;
}

/*
    Get the center position in y axis of the primary display
*/
int kengine::getDisplayCenterPosY(int height)
{
    return 1;
}

/*
    Get the number of monitors
*/
int kengine::getNumberOfMonitors()
{
    return 1;
}

/*
    Send QUIT message to indicate to the system that a thread has made a request to terminate (quit).
*/
void kengine::quitApplication(int returnCode) {}

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

    android_poll_source* source = nullptr;
    auto result = ALooper_pollOnce(0, nullptr, nullptr, (void**)&source);

    if (result == ALOOPER_POLL_ERROR) {
        std::string error = "ALooper_pollOnce returned an error" + std::to_string(result);
        K_LOG_OUTPUT_RAW(error.c_str());
        return;
    }

    // process this event
    if (source != nullptr)
        source->process(globalAppManager.m_android_app, source);
}

/*
    Getting OpenGL function address
*/
void* kengine::getGLFunctionAddress(std::string name)
{
    return nullptr;
}

/*
    runtime function to get opengl procedures extension
*/
int kengine::getAllGLProcedureAddress()
{
    return 1;
}

/*
*/
//int kengine::setVSync(int vsync)
//{
    //return eglSwapInterval ();
//    return 1;
//}


void androidHandleCmd(struct android_app* app, int32_t cmd)
{
    // https://developer.android.com/reference/games/game-activity/group/android-native-app-glue

    switch (cmd) {

    // command from main thread (java): a new ANativeWindow is ready for use.
    case APP_CMD_INIT_WINDOW:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD INIT WINDOW");

        if (app->window != nullptr) {
            K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD INIT WINDOW - app->window != nullptr");

            /*
                Segundo os exemplos oficiais do uso da NDK da Google,
                em alguns dispositivos, ANativeWindow pode ser recriada
                quando o app volta para o primeiro plano
            */
            if (kengine::globalAppManager.m_android_window.m_hWindow != app->window)
            {
                K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD INIT WINDOW - g_app.m_android_window != app->window");
                kengine::globalAppManager.m_android_window.m_hWindow = app->window;
            }

            globalUserEventsCallback->onWindowReady(&kengine::globalAppManager.m_android_window);
        }

        break;

    // command from main thread (java): the existing ANativeWindow needs to be terminated.
    case APP_CMD_TERM_WINDOW:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD TERM WINDOW");
        globalUserEventsCallback->onWindowDestroy();
        break;

    // Command from main thread (java): the current ANativeWindow has been resized.
    case APP_CMD_WINDOW_RESIZED:
    {
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD WINDOW RESIZED");

        std::string widthString = "ANDROID HANDLE CMD - width: " +
        std::to_string(ANativeWindow_getWidth(app->window));
        K_LOG_OUTPUT_RAW(widthString.c_str());

        std::string heightString = "ANDROID HANDLE CMD - height: " +
        std::to_string(ANativeWindow_getHeight(app->window));
        K_LOG_OUTPUT_RAW(heightString.c_str());

        globalUserEventsCallback->onResizeWindowEvent(ANativeWindow_getWidth(app->window),
        ANativeWindow_getHeight(app->window));

        break;
    }

    // command from main thread (java): the system needs that the current ANativeWindow be redrawn.
    case APP_CMD_WINDOW_REDRAW_NEEDED:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD WINDOW REDRAW NEEDED");
        break;

    // command from main thread (java): the content area of the window has changed
    case APP_CMD_CONTENT_RECT_CHANGED:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD CONTENT RECT CHANGED");
        break;

    // command from main thread (java): the app's activity window has gained input focus.
    case APP_CMD_GAINED_FOCUS:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD GAINED FOCUS");
        //        eng->ResumeSensors();
        // Start animation
        //        eng->has_focus_ = true;
        break;

    // command from main thread: the app's activity window has lost input focus.
    case APP_CMD_LOST_FOCUS:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD LOST FOCUS");
        //        eng->SuspendSensors();
        // Also stop animating.
        //        eng->has_focus_ = false;
        //        eng->DrawFrame();
        break;

    // command from main thread (java): the current device configuration has changed.
    case APP_CMD_CONFIG_CHANGED:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD CONFIG CHANGED");
        break;

    // command from main thread (java): the system is running low on memory.
    case APP_CMD_LOW_MEMORY:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD LOW MEMORY");
        // Free up GL resources
        break;

    // command from main thread (java): the app's activity has been started.
    case APP_CMD_START:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD START");
        break;

    // command from main thread (java): the app's activity has been resumed.
    case APP_CMD_RESUME:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD START");
        globalUserEventsCallback->onResumeEvent();
        break;

    // command from main thread (java): the existing ANativeWindow needs to be terminated.
    case APP_CMD_SAVE_STATE:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD SAVE STATE");
        break;

    // command from main thread (java): the app's activity has been paused.
    case APP_CMD_PAUSE:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD PAUSE");
        globalUserEventsCallback->onPauseEvent();
        break;

    // command from main thread (java): the app's activity has been stopped.
    case APP_CMD_STOP:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD STOP");
        break;

    // command from main thread (java): the app's activity is being destroyed, and waiting for the app thread to clean up and exit before proceeding.
    case APP_CMD_DESTROY:
        K_LOG_OUTPUT_RAW("ANDROID HANDLE CMD - APP CMD DESTROY");
        globalUserEventsCallback->onFinishEvent();
        break;

    // command from main thread (java): the app's insets have changed
//    case APP_CMD_WINDOW_INSETS_CHANGED:
//        break;
    }
}

int32_t androidHandleInput(android_app* app, AInputEvent* event)
{
    return 1;
}

#endif