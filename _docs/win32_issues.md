# Win32 Issues [em construção]

### Identificando a plataforma MS Windows no código (```WIN32``` vs ```_WIN32```)

Para evitar conflitos de nomes, é melhor utilizar ```_WIN32``` em vez de ```WIN32```.

- ```WIN32``` is defined by the SDK or build environment
- ```_WIN32``` is defined by the compiler

```
  #ifdef _WIN32
    // your WIN32 API code here
  #endif
```

Referências:
  - https://stackoverflow.com/questions/662084/whats-the-difference-between-the-win32-and-win32-defines-in-c
  - https://sourceforge.net/p/predef/wiki/OperatingSystems/
  - https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks


### MS Windows entry point - main x WinMain

Aplicações Windows que utilizam interface gráfica com janelas (_necessária para renderizar imagens com OpenGL_) tem o ponto de entrada de execução a partir da função ```WinMain()``` diferente de um programa convecional em C++ que tem o ponto de entrada de execução com a função ```main()```. Além disso, para definir a função ```WinMain```, é necessário também incluir o header ```windows.h```.

```
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>

  int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
  {
    return 0;
  }
```

Observe que isso também impede a utilização I/O padrão de C++ com ```std::cout``` e ```std::cin``` em consoles. Uma alternativa seria criar um console programaticamente da seguinta forma:

```
  AllocConsole();
  freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
  std::cout << "saida C++" << std::endl;
  FreeConsole();
```

Como especificar pelo CMake que o entry point da aplicação será com ```WinMain```?

A opção ```WIN32``` no comando ```add_executable``` de CMAKE especifica que deve ser utilizado a função ```WinMain``` em vez de ```main``` como ponto de entrada.

```
  add_executable(${APP_EXE} WIN32 main.cpp)
```

> **Nota:** Com esta opção, é necessário definir a função ```WinMain```, senão ocorrá um erro de linkedição:
```
error LNK2019: unresolved external symbol main referenced in function "int __cdecl __scrt_common_main_seh(void)" ( ?__scrt_common_main_seh@@YAHXZ)
```

Entretanto, ainda assim é possível criar uma janela de aplicaçã e tratar os eventos enviados pelo sistema operacional sem a necessidade de definir uma função ```WinMain```. Basta criar uma programa sem a opção WIN32 no CMake e chamar as mesmas funções de criação de janelas. A diferença agora é que dessa forma a aplicação sempre iniciará com um console. Portanto, uma segunda alternativa seria utilizar a opção WIN32 somente em build Release.

```
  add_executable(${APPNAME} ${SOURCE})
  set_target_properties(${APPNAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
```

Referências:
- https://cmake.org/cmake/help/v3.8/command/add_executable.html
- https://cmake.org/cmake/help/latest/prop_tgt/WIN32_EXECUTABLE.html#prop_tgt:WIN32_EXECUTABLE
- https://stackoverflow.com/questions/18709403/winmain-vs-main-c
- https://stackoverflow.com/questions/15543571/allocconsole-not-displaying-cout

# Inclusão de WIN32_LEAN_AND_MEAN para remover os warnings de compilação para MSVC 2019

A constante ```WIN32_LEAN_AND_MEAN``` foi introduzida para remover headers dentro do arquivo windows.h diminuindo o seu tamanho e aumentando a velocidade de compilação. Para obter mais detalhes de quais headers são removidos, basta verificar o arquivo windows.h e procurar pela constante ```WIN32_LEAN_AND_MEAN```.
Além disso, a constante remove diversos warnings no processo de compilação.

```
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
```

FonteS: https://devblogs.microsoft.com/oldnewthing/20091130-00/?p=15863#:~:text=The%20WIN32_LEAN_AND_MEAN%20symbol%20was%20introduced,a%20look%20at%20your%20windows.&text=The%20symbol%20was%20added%20as,Windows%20to%2032%2Dbit%20Windows.

# Diferenças de nomes para windows.h

Fonte: https://stackoverflow.com/questions/15466613/lowercase-windows-h-and-uppercase-windows-h-difference#:~:text=3%20Answers&text=There%20is%20no%20difference%2C%20and,(NTFS)%20are%20case%20insensitive.&text=Note%20that%20both%20modes%20preserve,to%20locate%20an%20existing%20file.


## Ordem de eventos das mensagens em MS Windows

###### Calling ```CreateWindowEx```:

- **WM_GETMINMAXINFO (0x0024)** - Sent to a window when the size or position of the window is about to change.
- **WM_NCCREATE (0x0081)** - Sent prior to the WM_CREATE message when a window is first created.
- **WM_NCCALCSIZE (0x0083)** - Sent when the size and position of a window's client area must be calculated.
- **WM_CREATE (0x0001)** - Sent when an application requests that a window be created by calling the CreateWindowEx or CreateWindow function.

###### Calling ```DestroyWindow```

- **WM_WINDOWPOSCHANGING (0x0046)** - Sent to a window whose size, position, or place in the Z order is about to change as a result of a call to the SetWindowPos function or another window-management function.
- **WM_WINDOWPOSCHANGED (0x0047)** - Sent to a window whose size, position, or place in the Z order has changed as a result of a call to the SetWindowPos function or another window-management function.
- **WM_IME_SETCONTEXT (0x0281)** - Sent to an application when a window is activated.
- **WM_IME_NOTIFY (0x0282)** - Sent to an application to notify it of changes to the IME window.
- **WM_DESTROY (0x0002)** - Sent when a window is being destroyed. It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
- **WM_NCDESTROY (0x0082)** - Notifies a window that its nonclient area is being destroyed.

###### Calling ```PostQuitMessage```

- **WM_QUIT (0x0012)** - Indicates a request to terminate an application, and is generated when the application calls the PostQuitMessage function.

###### Calling ```ShowWindow```:

- **WM_SHOWWINDOW (0x0018)** - Sent to a window when the window is about to be hidden or shown.
- **WM_WINDOWPOSCHANGING (0x0046)** - Sent to a window whose size, position, or place in the Z order is about to change as a result of a call to the SetWindowPos function or another window-management function.
- **WM_ACTIVATEAPP (0x001C)** - Sent when a window belonging to a different application than the active window is about to be activated.
- **WM_NCACTIVATE (0x0086)** - Sent to a window when its nonclient area needs to be changed to indicate an active or inactive state.
- **WM_GETICON (0x007F)** - Sent to a window to retrieve a handle to the large or small icon associated with a window.
- **WM_ACTIVATE (0x0006)** - Sent to both the window being activated and the window being deactivated.
- **WM_IME_SETCONTEXT (0x0281)** - Sent to an application when a window is activated.
- **WM_IME_NOTIFY (0x0282)** - Sent to an application to notify it of changes to the IME window.
- **WM_SETFOCUS (0x0007)** - Sent to a window after it has gained the keyboard focus.
- **WM_NCPAINT (0x0085)** - The WM_NCPAINT message is sent to a window when its frame must be painted.
- **WM_ERASEBKGND (0x0014)** - Sent when the window background must be erased (for example, when a window is resized).
- **WM_WINDOWPOSCHANGED (0x0047)** - Sent to a window whose size, position, or place in the Z order has changed as a result of a call to the SetWindowPos function or another window-management function.
- **WM_SIZE (0x0005)** - Sent to a window after its size has changed.
- **WM_MOVE (0x0003)** - Sent after a window has been moved.

###### Closing Window

- **WM_SYSCOMMAND (0x0112)** - A window receives this message when the user chooses a command from the Window menu (formerly known as the system or control menu) or when the user chooses the maximize button, minimize button, restore button, or close button.
- **WM_CLOSE (0x0010)** - Sent as a signal that a window or an application should terminate.


###### Window deactivated

- **WM_NCACTIVATE (0x0086)** - Sent to a window when its nonclient area needs to be changed to indicate an active or inactive state.
- **WM_ACTIVATE (0x0006)** - Sent to both the window being activated and the window being deactivated.
- **WM_ACTIVATEAPP (0x001C)** - Sent when a window belonging to a different application than the active window is about to be activated.
- **WM_KILLFOCUS (0x0008)** - Sent to a window immediately before it loses the keyboard focus.

###### Mouse Input

- **WM_NCHITTEST (0x0084)** - Sent to a window in order to determine what part of the window corresponds to a particular screen coordinate.
- **WM_SETCURSOR (0x0020)** - Sent to a window if the mouse causes the cursor to move within a window and mouse input is not captured.
- **WM_MOUSEMOVE (0x0200)** - Posted to a window when the cursor moves.
- **WM_NCMOUSEMOVE (0x00A0)** - Posted to a window when the cursor is moved within the nonclient area of the window.
- **WM_NCMOUSELEAVE (0x02A2)** - Posted to a window when the cursor leaves the nonclient area of the window specified in a prior call to TrackMouseEvent.
- **WM_NCLBUTTONDOWN (0x00A1)** - Posted when the user presses the left mouse button while the cursor is within the nonclient area of a window.
- **WM_CAPTURECHANGED (0x0215)** - Sent to the window that is losing the mouse capture.

###### ```Others```:

- **WM_DWMNCRENDERINGCHANGED (0x031F)** - Sent when the non-client area rendering policy has changed.
- **WM_PAINT (0x000F)** - The WM_PAINT message is sent when the system or another application makes a request to paint a portion of an application's window.
- **WM_SYSCOMMAND (0x0112)** - A window receives this message when the user chooses a command from the Window menu (formerly known as the system or control menu) or when the user chooses the maximize button, minimize button, restore button, or close button.


# Quiting application

The application must be destroy the Window before terminate the application. Error, unregisterWindow in Win32 because the windows was not destroyed


# OpenGL in MS Windows platform

On Windows, including Windows.h before including GL.h is required.

```
  #include <windows.h>
  #include <GL/gl.h>
```

Reference:
- https://stackoverflow.com/questions/75948278/why-is-windows-h-required-for-the-use-of-opengl-functions
