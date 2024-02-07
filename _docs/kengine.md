# K-Engine Game Engine

**K-Engine** é um projeto que se iniciou em 2020 com a motivação principal de estudar arquiteturas de motor de jogos e tecnologias relacionadas, assim como outros campos das áreas de Desenvolvimento de Jogos Digitais e Ciência da Computação de uma maneira mais profunda.

Aqui você encontrará todas as ideias de design e uma lista completa com todas as referências utilizadas no projeto.

> *K-Engine* tem um valor sentimental imenso para mim! Existem milhares de motores de jogos incríveis por aí que podem ser considerados melhores em diversos aspectos, porém, este projeto esteve presente em todas as fases da minha vida desde que iniciei meus estudos na área da Computação. Posso dizer com toda certeza que devo todo o meu conhecimento em Computação e Desenvolvimento de Jogos à **K-Engine**! <3


# Building K-Engine  

O projeto K-Engine possui o seu próprio CMAKE permitindo que o projeto possa ser compilado para as plataformas **Microsoft Windows** e **Linux** (e, futuramente, para Android).

A distribuição de *K-Engine* está na forma de uma biblioteca estática ```kengine.lib``` e um conjunto de arquivos headers (```core/inc/*.hpp```). Toda a estrutura de diretórios foi pensada para facilitar a compilação e obter facilmente os arquivos necessários para incluí-los em outros projetos.

> Nota: Possibilidade de geração da biblioteca na forma dinâmica (dll ou .so)

O projeto também gera um executável chamado ```game``` que é o exemplo principal que demonstra as funcionalidades da versão atual.

A estrutura conta também com diretórios separados com exemplos de uso, testes, avaliação de desempenho e documentação.

[imagem da estrutura de diretórios]

Arquivos necessários para utilizar k-engine em seus projetos:

- ```kengine.lib```
- arquivos headers (.hpp) em ```core/inc/```
