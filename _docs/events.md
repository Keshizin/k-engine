# K-Engine Runtime Lifetime and Callback Events

A partir do momento em que ```kengine::core::startMainLoop``` é chamado, K-Engine irá tomar o controle da execução do programa. Os **eventos de callback** foram criados para permitir que os usuários possam definir comportamentos personalizados que serão chamados implicitamente por K-Engine.

O **eventos de callback** são métodos da classe abstrata ```kengine::events_callback``` e o usuário deve criar uma classe derivada para definir comportamentos personalizados que serão chamados implicitamente por K-Engine.

Por que os eventos de callback são métodos de uma classe abstrata em vez de funções estáticas?

Vantagens:

- Permite trocar os eventos de callback apenas com ponteiros ```kengine::event_callback*```.
- O código fica mais organizado e facilita a manutenção.

Desvantagens:

- Overhead no uso de funções virtuais (funções estáticas podem oferecer melhor desempenho)
- Necessidade de sobrescreever todas as funções virtuais

Após sobrecarregar todos os métodos de ```kengine::events_callback``` basta configurar em ```kengine::core```:

```
  class splash : public kengine::events_callback
  {
  public:
    void update(int64_t frameTime) {
      // your game logic that execute per frame
    }

    // override other methods...
  };

  splash myEvents;
  kengine::core m_engine;
  m_engine->setEventsCallback(&myEvents);
```

# Eventos de callback

```void onWindowReady(window* window)```  
Evento de callback chamado dentro do método de criação de uma janela de aplicação (```kengine::window::create```) momento antes dele retornar com sucesso.

```void beforeMainLoopEvent()```  
Evento de callback chamado antes da execução do loop principal iniciar.


```void update(int64_t frameTime)```  
Evento de callback chamado uma vez a cada iteração do loop principal. Este evento é chamado após tratamento de mensagens (message pump) do Sistema Operacional.

```void onFinishEvent()```  
Evento de callback chamado quando a janela recebe a mensagem que a aplicação está sendo encerrada (após receber a mensagem que a janela foi destruída).

```onWindowDestroy()```  
Evento de callback chamado quando a janela está sendo destruída.


		/*
			Callback event called when the window is deactivated (e.g. minimized window)
		*/
		virtual void onPauseEvent() = 0;

		/*
			Callback event called when the window is activated
		*/
		virtual void onResumeEvent() = 0;

		/*
			Callback event called when the user clicks the window's close button
		*/
		virtual void closeButtonEvent() = 0;

		/*
			Callback event called when the window is being resized
		*/
		virtual void onResizeWindowEvent(const int width, const int height) = 0;

		/*
			Callback event called when the window is being moved
		*/
		virtual void onMoveWindowEvent(const int x, const int y) = 0;

		/*
			Callback event called on mouse move event
		*/
		virtual void onMouseMotionEvent(const int x, const int y) = 0;

		/*
			Callback event called on mouse button event
		*/
		virtual void onMouseButtonEvent(const int button, const MOUSE_ACTION& state, const int x, const int y) = 0;

		/*
			Callback event called on keyboard key event
		*/
		virtual void onKeyboardEvent(const unsigned long long key, const int state) = 0;

		/*
			Callback event called on special keyboard keys event like shift, ctrl, etc
		*/
		virtual void onKeyboardSpecialEvent(const unsigned long long key, const int state) = 0;

		/*
			Callback event for debug message
		*/
		virtual void debugMessage(const std::string& msg) = 0;

		/*
			Custom message pump handler
			This callback is specific for Win32 window procedure
		*/
		virtual void customWindowProcedure(void* param1, void* param2, void* param3, void* param4) = 0;
