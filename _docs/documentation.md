# K-Engine

What is K-Engine?

## Personalização de uma janela de aplicação K-Engine!

Tipos de janelas:

- K_WINDOW_SPLASH
- K_WINDOW_DEFAULT
- K_WINDOW_NO_SYS
- K_WINDOW_WINDOWED_FULLSCREEN
- K_WINDOW_COMPLETE

## K-Engine Events

- frameEvent
- mouseEvent
- mouseMotionEvent
- keyboardEvent
- keyboardSpecialEvent
- resizeWindowEvent
- moveWindowEvent
- finishAfterEvent
- finishBeforeEvent
- resumeEvent
- pauseEvent
- beforeMainLoopEvent
- afterMainLoopEvent
- createWindowEvent

### K-Events' calls order

Eventos durante a criação de uma janela de aplicação:

1. createWindowEvent
2. resizeWindowEvent (também é chamado quando a janela é redimensionada)
3. moveWindowEvent (também é chamado quando a posição da janela é alterada)

Eventos durante a destruição de uma janela de aplicação:
1. finishBeforeEvent

Eventos durante a interação do mouse:
- mouseMotionEvent
- mouseEvent

Eventos durante a interação do teclado:
- keyboardEvent
- keyboardSpecialEvent

Eventos durante o main loop de K-Engine:
- beforeMainLoopEvent
- frameEvent
- resumeEvent
- pauseEvent
- finishAfterEvent
- afterMainLoopEvent

### Win32's messages order

Eventos chamados durante a execução da função CreateWindowEx (Win32):

- WM_GETMINMAXINFO (36)
  - Informações a respeito da janela (importante para múltiplos monitores)
- WM_NCCREATE (129)
- WM_NCCALCSIZE (131)
- WM_CREATE (1)
  - createWindowEvent
- WM_SIZE (5)
  - resizeWindowEvent
- WM_MOVE (3)
  - moveWindowEvent

Eventos chamados durante a execução da função ShoWindow(Win32):

- WM_SHOWWINDOW (24)
- WM_WINDOWPOSCHANGING (70)
- WM_ACTIVEAPP (28)
  - Enviado quando uma janela pertencente a um aplicativo diferente da janela ativa está prestes a ser ativada. A mensagem é enviada para o aplicativo cuja janela está sendo ativada e para o aplicativo cuja janela está sendo desativada.
- WM_NCACTIVATE (134)
- WM_GETICON (127)
- WM_ACTIVATE (6)
  - Se a janela está sendo ativa é chamada resumeEvent. Se a janela estiver sendo desativada, pauseEvent.
- WM_IME_SETCONTEXT (641)
- WM_IME_NOTIFY (642)
- WM_SETFOCUS (7)
  - Enviado para uma janela após obter o foco do teclado.
- WM_NCPAINT (133)
- WM_ERASEBKGND (20)
- WM_WINDOWPOSCHANGED (71)

Eventos durante a destruição de uma janela de aplicação Win32:
- WM_CLOSE (16)
  - finishBeforeEvent
- WM_DESTROY (2)
- WM_NCDESTROY (130)

Eventos chamados durante a interação do mouse com a aplicação:

- WM_NCHITTEST (132)
- WM_SETCURSOR (32)
- WM_MOUSEMOVE/WM_MOUSEFIRST (512)
  - mouseMotionEvent
- WM_NCMOUSEMOVE (160)
- WM_NCMOUSELEAVE (674)
- WM_NCLBUTTONDOWN (161)
- WM_CAPTURECHANGED (533)
- WM_LBUTTONDOWN
  - mouseEvent
- WM_LBUTTONUP
  - mouseEvent
  - WM_MBUTTONDOWN
	- mouseEvent
- WM_MBUTTONUP
	- mouseEvent
- WM_RBUTTONDOWN
	- mouseEvent
- WM_RBUTTONUP
	- mouseEvent

Eventos chamados durante a interação do teclado com a aplicação:

- WM_KEYDOWN (256)
  - keyboardEvent
- WM_KEYUP (257)
  - keyboardEvent
- WM_CHAR (258)
- WM_SYSCOMMAND
- WM_SYSKEYDOWN
  - keyboardSpecialEvent
- WM_SYSKEYUP
  - keyboardSpecialEvent

Alguns eventos acima e outros eventos podem ser chamados dependendo da interação com a janela da aplicação:

- WM_DWMNCRENDERINGCHANGED (799)
- WM_PAINT (15)
- WM_NCACTIVATE (134)
- WM_KILLFOCUS (8)

## KETimer

Nível de precisão: 2000 ciclos ~ 15000 ciclos
