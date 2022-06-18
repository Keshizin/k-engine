# K-Engine Devlog

Yo! Meu nome é Keshi e estou aqui para compartilhar a minha loooonga jornada de desenvolvimento de K-Engine!

Vocês devem estar se perguntando por que escrever mais um motor de jogo ou o que ele tem de especial no meio de tantos outros motores incríveis que tem por ai? Pois é, eu fiz essas perguntas a mim mesmo milhões de vezes e sempre cheguei a mesma resposta:

> K-Engine tem um valor sentimental imenso para mim. Apesar de estar todo remendado parecendo um Frankenstein, eu me sinto realizado e tenho puro prazer de continuar o seu desenvolvimento. K-Engine é um projeto longo que esteve presente em todas as fases da minha vida, seja ruim ou boa. Posso dizer com toda certeza que eu devo todo o meu conhecimento em Computação e Desenvolvimento de Jogos a K-Engine! <3

Neste devlog, pretendo incluir as minhas ideias e explicar os motivos de ter tomado certas decisões e escolhas de design para cada componente que faz parte de K-Engine. Irei compartilhar detalhes da minha experiência em desenvolver um motor de jogo, lições aprendidas durante o desenvolvimento e todo conhecimento teórico adquirido que fundamentaram a construção de K-Engine. No final deste devlog, contém uma seção com a lista de todas as referências utilzadas no projeto.

Espero que este devlog seja útil de alguma forma e que possa inspirar outras pessoas que tem o desejo de criar um motor de jogo do zero.


## Rendering System

### Visualização e Projeção

Como funciona o mapeamento dos modelos definidos em um sistema de coordenada tridimensional para o sistema de coordenadas do monitor.
O que são normalized homogenous coordinates [-1.0, 1.0] e normalized device coordinates (NDC)

1. modeling transformation (eye space)
2. viewing transformation (eye space)
3. projection transformation
4. viewport transformation

Transformações Lineares e Matrizes
a importância das coordenadas homogeneas para projeção perspectiva (clip coordinates)

Viewing frustum e Orthographic Viewing model


### Camera

Implementação de LookAt
- operações matematicas para criar o sistema de coordenadas local
  - algebra linear (vetores, normalização, dot product, cross product, soma e subtração de vetores)
- Construção de matriz 4x4 para transformações lineares da camera


# References

- Gregory, J. Game Engine Architecture. 3rd ed. Boca Raton: CRC Press, 2018.
- Kessenich, J. Sellers, G. Shreiner, D. OpenGL Programming Guide. 9th ed. Addison-Wesley, 2016.
- Stroustrup, B. The C++ Programming Language. 4th ed. MA: Addison-Wesley, 2013.
- Scratchapixel 2.0. Disponível em https://www.scratchapixel.com/.
