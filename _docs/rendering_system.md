# K-Engine Rendering System

O sistema de renderização é responsável por receber dados geométricos, inicializar shaders e realizar a renderização.

É recomendável que o sistema de renderização seja criado no evento de callback ```onWindowReady```. Nas plataformas Android, a janela é criada implicitamente pelo Android Surface. Portanto, este evento foi criado especificamente para notificafar o usuário quando a janela estiver disponível, já que um sistema de renderização só existe se uma janela existir.

```
kengine::rendering_system m_renderingSystem; // declared in somewhere

void demo::game::onWindowReady(kengine::window* window)
{
  kengine::compatibility_profile profile;
  profile.contextFlag = kengine::CONTEXT_FLAG::CONTEXT_DEBUG_BIT_ARB;
  profile.profileMask = kengine::CONTEXT_FLAG::CONTEXT_CORE_PROFILE_BIT_ABR;

  if (!m_renderingSystem.init(window, kengine::RENDERING_TYPE::OPENGL, profile)) {
    // tratar o erro se init falhar
  }
```

# Notas de desenvolvimento

kengine::compatibility_profile é uma estrutura específica de OpenGL. Ela contém parametros de configuração relacionada ao contexto OpenGL.

Talvez não seja uma boa ideia ela estar tão acoplada dessa forma com a classe kengine::rendering_system. Necessário rever esta parte quando incluir uma nova API como Vulkan.

Construtor: rendering_system é uma classe que só existe se uma janela de aplicação existir. Portanto não faz sentindo um objeto desta classe existir sem estar relacionada diretamente com uma janela de aplicação. Por isso o construtor padrão deve ser removido. Por outro lado, para criar o contexto de renderização, deve permitir que o programador possa configurar parametros adicionais. Por isso existe o método init.
