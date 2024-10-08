#include <iostream>
#include <cstring>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include <GL/gl.h>
#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

static bool isExtensionSupported(const char *extList, const char *extension) {
       const char *start;
       const char *where, *terminator;

       where = strchr(extension, ' ');
       if (where || *extension == '\0') {
              return false;
       }

       for (start=extList;;) {
              where = strstr(start, extension);

              if (!where) {
                     break;
              }

              terminator = where + strlen(extension);

              if ( where == start || *(where - 1) == ' ' ) {
                     if ( *terminator == ' ' || *terminator == '\0' ) {
                            return true;
                     }
              }      

              start = terminator;
       }

       return false;
}

int main(int argc, char** argv) {
       Display* display;
       Window window;
       Screen* screen;
       int screenId;
       XEvent ev;

       // Open the display
       display = XOpenDisplay(NULL);
       if (display == NULL) {
              std::cout << "Could not open display\n";
              return 1;
       }
       screen = DefaultScreenOfDisplay(display);
       screenId = DefaultScreen(display);

       
       // Check GLX version
       GLint majorGLX, minorGLX = 0;
       glXQueryVersion(display, &majorGLX, &minorGLX);
       if (majorGLX <= 1 && minorGLX < 2) {
              std::cout << "GLX 1.2 or greater is required.\n";
              XCloseDisplay(display);
              return 1;
       }
       else {
              std::cout << "GLX client version: " << glXGetClientString(display, GLX_VERSION) << '\n';
              std::cout << "GLX client vendor: " << glXGetClientString(display, GLX_VENDOR) << "\n";
              std::cout << "GLX client extensions:\n\t" << glXGetClientString(display, GLX_EXTENSIONS) << "\n";

              std::cout << "GLX server version: " << glXQueryServerString(display, screenId, GLX_VERSION) << "\n";
              std::cout << "GLX server vendoe: " << glXQueryServerString(display, screenId, GLX_VENDOR) << "\n";
              std::cout << "GLX server extensions:\n\t " << glXQueryServerString(display, screenId, GLX_EXTENSIONS) << "\n";
       }

       GLint glxAttribs[] = {
              GLX_X_RENDERABLE    , True,
              GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
              GLX_RENDER_TYPE     , GLX_RGBA_BIT,
              GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
              GLX_RED_SIZE        , 8,
              GLX_GREEN_SIZE      , 8,
              GLX_BLUE_SIZE       , 8,
              GLX_ALPHA_SIZE      , 8,
              GLX_DEPTH_SIZE      , 24,
              GLX_STENCIL_SIZE    , 8,
              GLX_DOUBLEBUFFER    , True,
              None
       };
       
       int fbcount;
       GLXFBConfig* fbc = glXChooseFBConfig(display, screenId, glxAttribs, &fbcount);
       if (fbc == 0) {
              std::cout << "Failed to retrieve framebuffer.\n";
              XCloseDisplay(display);
              return 1;
       }
       std::cout << "Found " << fbcount << " matching framebuffers.\n";

       // Pick the FB config/visual with the most samples per pixel
       std::cout << "Getting best XVisualInfo\n";
       int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
       for (int i = 0; i < fbcount; ++i) {
              XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
              if ( vi != 0) {
                     int samp_buf, samples;
                     glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                     glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );
                     //std::cout << "  Matching fbconfig " << i << ", SAMPLE_BUFFERS = " << samp_buf << ", SAMPLES = " << samples << ".\n";

                     if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) ) {
                            best_fbc = i;
                            best_num_samp = samples;
                     }
                     if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                            worst_fbc = i;
                     worst_num_samp = samples;
              }
              XFree( vi );
       }
       std::cout << "Best visual info index: " << best_fbc << "\n";
       GLXFBConfig bestFbc = fbc[ best_fbc ];
       XFree( fbc ); // Make sure to free this!

       XVisualInfo* visual = glXGetVisualFromFBConfig( display, bestFbc );

       if (visual == 0) {
              std::cout << "Could not create correct visual window.\n";
              XCloseDisplay(display);
              return 1;
       }
       
       if (screenId != visual->screen) {
              std::cout << "screenId(" << screenId << ") does not match visual->screen(" << visual->screen << ").\n";
              XCloseDisplay(display);
              return 1;

       }

       // Open the window
       XSetWindowAttributes windowAttribs;
       windowAttribs.border_pixel = BlackPixel(display, screenId);
       windowAttribs.background_pixel = WhitePixel(display, screenId);
       windowAttribs.override_redirect = True;
       windowAttribs.colormap = XCreateColormap(display, RootWindow(display, screenId), visual->visual, AllocNone);
       windowAttribs.event_mask = ExposureMask;

       // (!)
       window = XCreateWindow(display, RootWindow(display, screenId), 0, 0, 320, 200, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

       // Create GLX OpenGL context
       glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
       glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

       const char *glxExts = glXQueryExtensionsString( display,  screenId );
       std::cout << "Late extensions:\n\t" << glxExts << "\n\n";
       if (glXCreateContextAttribsARB == 0) {
              std::cout << "glXCreateContextAttribsARB() not found.\n";
       }
       
       int context_attribs[] = {
              GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
              GLX_CONTEXT_MINOR_VERSION_ARB, 2,
              GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
              None
       };

       GLXContext context = 0;
       if (!isExtensionSupported( glxExts, "GLX_ARB_create_context")) {
              context = glXCreateNewContext( display, bestFbc, GLX_RGBA_TYPE, 0, True );
       }
       else {
              context = glXCreateContextAttribsARB( display, bestFbc, 0, true, context_attribs );
       }
       XSync( display, False );

       // Verifying that context is a direct context
       if (!glXIsDirect (display, context)) {
              std::cout << "Indirect GLX rendering context obtained\n";
       }
       else {
              std::cout << "Direct GLX rendering context obtained\n";
       }
       glXMakeCurrent(display, window, context);

       std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << "\n";
       std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << "\n";
       std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
       std::cout << "GL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

       // Show the window
       XClearWindow(display, window);
       XMapRaised(display, window);

       // Set GL Sample stuff
       glClearColor(0.5f, 0.6f, 0.7f, 1.0f);

       // Enter message loop
       while (true) {
              XNextEvent(display, &ev);
              if (ev.type == Expose) {
                     XWindowAttributes attribs;
                     XGetWindowAttributes(display, window, &attribs);
                     glViewport(0, 0, attribs.width, attribs.height);
              }
              
              // OpenGL Rendering
              glClear(GL_COLOR_BUFFER_BIT);

              glBegin(GL_TRIANGLES);
                     glColor3f(  1.0f,  0.0f, 0.0f);
                     glVertex3f( 0.0f, -1.0f, 0.0f);
                     glColor3f(  0.0f,  1.0f, 0.0f);
                     glVertex3f(-1.0f,  1.0f, 0.0f);
                     glColor3f(  0.0f,  0.0f, 1.0f);
                     glVertex3f( 1.0f,  1.0f, 0.0f);
              glEnd();

              // Present frame
              glXSwapBuffers(display, window);
       }

       // Cleanup GLX
       glXDestroyContext(display, context);

       // Cleanup X11
       XFree(visual);
       XFreeColormap(display, windowAttribs.colormap);
       XDestroyWindow(display, window);
       XCloseDisplay(display);
       return 0;
}