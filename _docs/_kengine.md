# K-Engine Game Engine [EM CONSTRUÇÃO]

**K-Engine** é um projeto que se iniciou em 2020 com a motivação principal de estudar arquiteturas de motor de jogos e tecnologias relacionadas, assim como outros campos das áreas de Desenvolvimento de Jogos Digitais e Ciência da Computação de uma maneira mais profunda.

Aqui você encontrará todas as ideias de design e uma lista com todas as referências utilizadas no projeto.

> *K-Engine* tem um valor sentimental imenso para mim! Existem milhares de motores de jogos incríveis por aí que podem ser considerados melhores em diversos aspectos, porém, este projeto esteve presente em todas as fases da minha vida desde que iniciei meus estudos na área da Computação. Posso dizer com toda certeza que devo todo o meu conhecimento em Computação e Desenvolvimento de Jogos à **K-Engine**! <3

# K-Engine Architecture Design

Meu interesse inicial pelo desenvolvimento de jogos surgiu ao ler o livro **"OpenGL: Uma Abordagem Prática e Objetiva"**, de _Marcelo Cohen_ e _Isabel Harb_, ambos professores de Computação Gráfica na PUC-RS (Pontifícia Universidade Católica do Rio Grande do Sul). A obra utilizava a biblioteca [**GLUT (OpenGL Utility Toolkit)**](https://www.opengl.org/resources/libraries/glut/glut_downloads.php) para ilustrar diversos conceitos de Computação Gráfica de forma didática. Um dos momentos mais marcantes da leitura foi desenvolver uma sala de aula virtual em 3D, com iluminação baseada no modelo Phong e navegação em primeira pessoa.

A partir deste livro, passei a explorar outras bibliotecas e frameworks para substituir GLUT, como, por exemplo [**GLFW**](https://www.glfw.org/). Percebi que todos eles utilizavam o conceito de **funções de callback**, onde a biblioteca ou framework controla o loop principal da aplicação, enquanto o programador define funções de callback personalizadas, que são chamadas implicitamente durante a sua execução. A principal desvantagem é a limitação no controle da execução do programa, especialmente quando a biblioteca utilizada é mais restrita em funcionalidades.

Querendo entender melhor o funcionamento interno destas bibliotecas, encontrei dois livros sobre programação de jogos utilizando a API [**Win32**](https://learn.microsoft.com/pt-br/windows/win32/desktop-programming) e **DirectX**: o livro **"Desenvolvimento de Jogos Eletrônicos: Teoria e Prática"**, de _Alexandre Souza Perucia et al._, e o livro **"Game Programming in 25 Hours"**, de _Michael Morrison_. Ambos os livros me deram a primeira introdução de uma arquitetura de motor de jogos.

A partir deste ponto, tomei a decisão que queria construir o meu próprio motor de jogo multiplataforma utilizando apenas a API Win32 para Microsoft Windows e LibX para Linux. Como a proposta inicial era garantir compatibilidade entre diferentes sistemas operacionais, optei por utilizar OpenGL em vez de DirectX.

[arquitetura inicial de K-Engine: core, callback_events, os_api_wrapper]

Com o objetivo definido, eu precisva me aprofundar mais sobre OpenGL. Dois livros foram essenciais: **"OpenGL Programming Guide 8th edition"** e **"OpenGL 4 Shading Language Cookbook 3rd edition"**. O segundo livro abriu mais portas ao introduzir o mundo da programação de shaders e animação. Além disso, comecei a ler o livro **"Game Engine Architecture, 3rd edition"** de _Jason Gregory_ para me aprofundar mais sobre arquitetura de motor de jogos. Jason Gregory é lead programmer na **Naughty Dog** e também professor universtário na univerdade de California.

# Arquitetura

[Framework vs biblioteca]
[Eventos de callbacks (events_callback.hpp)]

- [K-Engine Core](core.md)
- [Mesh](mesh.md)
- [Rendering System](rendering_system.md)

##### **kengine::rendering_context**  

Em OpenGL, existe um conceito de contexto que está relacionada a uma janela de aplicação e, consequentemente, ao processo.

[mostrar a pipeline: OpenGL API -> OpenGL context -> Window -> GDI -> Graphics Driver -> Monitor]

Esta classe é abstrata porque cada sistema operacional possui a sua própria maneira de criar um contexto de renderização. Além disso, a própria janela de aplicação também especifíca de cada sistema operacional. Portanto, a ideia é que a interface das classes ```rendering_context``` e ```window``` devem ser implementadas de acordo com a plataforma. O fato, é que uma janela de aplicação pode existir sem o contexto de renderização, mas um contexto de renderização não pode existir sem uma janela.

Uma possível solução para evitar chamadas de métodos virtuais (há um custo de desempenho e memória) é, em vez de criar uma classe abstrata, criar a interface da classe que será comum em todas as plataformas e utilizar comandos de pré-processador (```#ifdef```) para implementar versões dos métodos da classe para cada plataforma. Entretanto, cada paltaforma pode exigir alguns métodos ou parâmetros adicionais que não são necessários em outras plataformas, fazendo com que a classe fique com métodos adicionais não utilizadas. Apesar do custo de desempenho, classe abstrata permite uma organização mais limpa e mais fácil de manter.

Atenção ao permitir copy and move semantics.

# Notas de desenvolvimento

A classe ```kengine::window``` não faz parte da classe ```kengine::core``` porque ela está relacionada mais ao sistema operacional do que o motor de jogo em si. Inicialmente, a ```kengine::window``` era um membro de ```kengine::core``` para que a posição (x, y) e o tamanho (width e height) da janela pudessem ser obtidos diretamente, mas quando foi iniciado o desenvolvimento na plataforma Android (onde a janela ```ANativeWindow``` é criada e gerenciada implicitamente) foi percebido que manter as duas classes de forma independente sem relacionamento de composição faz mais sentido.

Existe um ponto de design que precisa ser melhorado na classe abstrata ```kengine::events_callback``` que é a quantidade de métodos virtuais que precisam ser sobrescritos pelo usuário da classe. Uma possível solução é para os métodos menos utilizados não declarar como função virtual pura (```=0```). Uma outra alternativa seria tornar todos os métodos de ```kengine::events_callback``` como funções globais. Uma desvantagem é que enquanto que poderiam existir diversas classes derivadas de ```kengine::events_callback``` com suas respectivas definções de métodos virtuais, as funções globais podem ser definidas apenas uma vez.

No arquivo ```gl_wrapper.cpp``` existe uma função chamada ```getAllProcedures``` para obter as implementações das funções OpenGL. É necessário uma refatoração na verificação dos ponteiros de função.

As mensagens de erro em ```gl_wrapper.cpp``` precisam ser removidas e retornar apenas códigos de erros.

Para a classe ```rendering_context``` criar novos métodos focados em desempenho (e inline também). Por exemplo, o método ```makeCurrent``` poderia não ter as validações de erros.

OpenGL Wrapper
Decisão utilizar comandos OpenGL DSA (glCreate) ou glGen para buffer objects. glGen é mais portável pois funciona em versões anteriores. Criar um metodo sobrecarregado para utilizar a versão glCreate

kengine::mesh
Escolha de std::unordered_map vs std::vector vs std::bitset

Inserir location 0 e 4 por exemplo
