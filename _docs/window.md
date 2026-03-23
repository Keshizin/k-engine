# K-Engine Window Management


```
  #include <os_api_wrapper.hpp>

  kengine::window* myWindow;
```

> Nota: A classe ```kengine::window``` não faz parte da classe ```kengine::core``` porque ela está relacionada mais ao sistema operacional do que o motor de jogo em si. Inicialmente, a ```kengine::window``` era um membro de ```kengine::core``` para que a posição (x, y) e o tamanho (width e height) da janela pudessem ser obtidos diretamente, mas quando foi iniciado o desenvolvimento na plataforma Android (onde a janela ```ANativeWindow``` é criada e gerenciada implicitamente) foi percebido que manter as duas classes de forma independente sem relacionamento de composição faz mais sentido.
