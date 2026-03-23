# K-Engine [EM CONSTRUÇÃO]

**K-Engine** is a open source game engine written in C++ developed for my undergraduate thesis in Computer Science (2020).

The main motivation for this project is to study game engine architectures and its technologies, as well as understand other fields of game development and computer science in a deeper way.

Here you will find all design ideas and a complete list of all references used in this project.

> *K-Engine* tem um valor sentimental imenso para mim! Existem milhares de motores de jogos incríveis por aí que podem ser considerados melhores em diversos aspectos, porém, este projeto esteve presente em todas as fases da minha vida desde que iniciei meus estudos na área da Computação. Posso dizer com toda certeza que devo todo o meu conhecimento em Computação e Desenvolvimento de Jogos à **K-Engine**! <3

**Table of Contents**

- [Building](building.md)  
- [K-Engine architecture](arch.md)  
- [K-Engine Runtime Lifecycle and Callback Events](events.md)  
- [K-Engine Window Management](window.md)  
- [K-Engine Rendering System](rendering_system.md)  
- [K-Engine Win32](win32.md)

# Development Notes

Meu interesse inicial pelo desenvolvimento de jogos surgiu ao ler o livro **"OpenGL: Uma Abordagem Prática e Objetiva"**, de _Marcelo Cohen_ e _Isabel Harb_, ambos professores da PUC-RS (Pontifícia Universidade Católica do Rio Grande do Sul). A obra utiliza **OpenGL** para ilustrar diversos conceitos de Computação Gráfica de forma prática e simples. No final, o livro incetiva o leitor a desenvolver um projeto de uma sala virtual 3D interativa com todos os conceitos explicados no livro.

A partir deste livro, passei a explorar outras bibliotecas e frameworks para substituir [**GLUT (OpenGL Utility Toolkit)**](https://www.opengl.org/resources/libraries/glut/glut_downloads.php) como, por exemplo, [**GLFW**](https://www.glfw.org/) e [**SDL2**](https://www.libsdl.org/). Estas bibliotecas controlavam o loop principal da aplicação e utilizavam o conceito de **funções de callback** para permitir que os programadores pudessem personalizar o comportamento da aplicação. As funções de callback eram chamadas implicitamente pelo próprio framework.

Para entender melhor o funcionamento interno destas bibliotecas, encontrei dois livros sobre programação de jogos utilizando a API [**Win32**](https://learn.microsoft.com/pt-br/windows/win32/desktop-programming) e **DirectX**: o livro **"Desenvolvimento de Jogos Eletrônicos: Teoria e Prática"**, de _Alexandre Souza Perucia et al._ e o livro **"Game Programming in 25 Hours"**, de _Michael Morrison_. Ambos os livros me deram a primeira introdução de uma arquitetura de motor de jogos e a utilização da API **Win32**.

A partir deste ponto, tomei a decisão de construir o meu próprio motor de jogo multiplataforma utilizando apenas a API Win32 para Microsoft Windows e LibX para Linux. Como a proposta inicial era garantir compatibilidade entre diferentes sistemas operacionais, optei por utilizar OpenGL em vez de DirectX.

# OpenGL

A partir da versão 4.4 e 4.5 foram introduzidos novos conceitos para melhorar o desempenho: immutable objects e direct state access. Entretanto, para manter compatiblidade com a versão Android, todas as funções OpenGL foram baseadas a partir da versão 3.0+. Portanto, a implementação padrão do rendering system não utiliza immutable objects e direct state acess.

??
[arquitetura inicial de K-Engine: core, callback_events, os_api_wrapper]

Com o objetivo definido, eu precisva me aprofundar mais sobre OpenGL. Dois livros foram essenciais: **"OpenGL Programming Guide 8th edition"** e **"OpenGL 4 Shading Language Cookbook 3rd edition"**. O segundo livro abriu mais portas ao introduzir o mundo da programação de shaders e animação. Além disso, comecei a ler o livro **"Game Engine Architecture, 3rd edition"** de _Jason Gregory_ para me aprofundar mais sobre arquitetura de motor de jogos. Jason Gregory é lead programmer na **Naughty Dog** e também professor universtário na univerdade de California.

# Arquitetura

[Framework vs biblioteca]

- [Mesh](mesh.md)
- [Rendering System](rendering_system.md)

##### **kengine::rendering_context**  

Em OpenGL, existe um conceito de contexto que está relacionada a uma janela de aplicação e, consequentemente, ao processo.

[mostrar a pipeline: OpenGL API -> OpenGL context -> Window -> GDI -> Graphics Driver -> Monitor]

Esta classe é abstrata porque cada sistema operacional possui a sua própria maneira de criar um contexto de renderização. Além disso, a própria janela de aplicação também especifíca de cada sistema operacional. Portanto, a ideia é que a interface das classes ```rendering_context``` e ```window``` devem ser implementadas de acordo com a plataforma. O fato, é que uma janela de aplicação pode existir sem o contexto de renderização, mas um contexto de renderização não pode existir sem uma janela.

Uma possível solução para evitar chamadas de métodos virtuais (há um custo de desempenho e memória) é, em vez de criar uma classe abstrata, criar a interface da classe que será comum em todas as plataformas e utilizar comandos de pré-processador (```#ifdef```) para implementar versões dos métodos da classe para cada plataforma. Entretanto, cada paltaforma pode exigir alguns métodos ou parâmetros adicionais que não são necessários em outras plataformas, fazendo com que a classe fique com métodos adicionais não utilizadas. Apesar do custo de desempenho, classe abstrata permite uma organização mais limpa e mais fácil de manter.

Atenção ao permitir copy and move semantics.

??
Existe um ponto de design que precisa ser melhorado na classe abstrata ```kengine::events_callback``` que é a quantidade de métodos virtuais que precisam ser sobrescritos pelo usuário da classe. Uma possível solução é para os métodos menos utilizados não declarar como função virtual pura (```=0```). Uma outra alternativa seria tornar todos os métodos de ```kengine::events_callback``` como funções globais. Uma desvantagem é que enquanto que poderiam existir diversas classes derivadas de ```kengine::events_callback``` com suas respectivas definções de métodos virtuais, as funções globais podem ser definidas apenas uma vez.

No arquivo ```gl_wrapper.cpp``` existe uma função chamada ```getAllProcedures``` para obter as implementações das funções OpenGL. É necessário uma refatoração na verificação dos ponteiros de função.

As mensagens de erro em ```gl_wrapper.cpp``` precisam ser removidas e retornar apenas códigos de erros.

Para a classe ```rendering_context``` criar novos métodos focados em desempenho (e inline também). Por exemplo, o método ```makeCurrent``` poderia não ter as validações de erros.

OpenGL Wrapper
Decisão utilizar comandos OpenGL DSA (glCreate) ou glGen para buffer objects. glGen é mais portável pois funciona em versões anteriores. Criar um metodo sobrecarregado para utilizar a versão glCreate

kengine::mesh
Escolha de std::unordered_map vs std::vector vs std::bitset

Inserir location 0 e 4 por exemplo

- Linux
  - https://man7.org/linux/man-pages/man3/clock_gettime.3.html
  - https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/

C++17 para utilizar std::filesystem
