https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions

https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)

# Platform issues

## MS Windows platform

On Windows, including Windows.h before including GL.h is required.

```
  #include <windows.h>
  #include <GL/gl.h>
```

Reference:
- https://stackoverflow.com/questions/75948278/why-is-windows-h-required-for-the-use-of-opengl-functions
