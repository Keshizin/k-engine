# K-Engine! Starting Guide

# 1. Creating your own events callback

Override the event callback methods for your use.

```
  class splash : public kengine::events_callback
  {
  public:
    void update(int64_t frameTime) {
      // your game logic that execute per frame
    }

    // override other methods...
  }
```

> **Note:** É necessário sobreescrever todos os métodos de ```kengine::events_callback```.

Após sobrecarregar todos os métodos de ```kengine::events_callback``` basta configurar em ```kengine::core```:

```
  kengine::core m_engine;
  splash myEvents;
  m_engine->setEventsCallback(&myEvents);
```

# 2. Criando a Janela de Aplicação

# 3. Iniciando o main loop principal

A partir do momento em que ```kengine::core::startMainLoop``` é chamado, K-Engine irá tomar o controle da execução do programa.

```
  m_engine->startMainLoop();
```

# 4. Utilize os eventos de callback para criar objetos, renderizar, etc
