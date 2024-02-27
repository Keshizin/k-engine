# Building K-Engine!

## Pre-requisites
- CMake
- Lua 5.4.6

## Sistema de Building

O projeto K-Engine possui o seu próprio **CMAKE** permitindo que o projeto possa ser compilado para as plataformas **Microsoft Windows** e **Linux** (e, futuramente, para **Android**) e também para permitir configurações específicas do projeto.

Para cada plataforma existem scripts para realizar build do projeto.

- ```build/win/win-build.bat```
- ```build/linux/linux-build.sh```

Estes scripts, além de executar o comando **cmake** e **make** para cada plataforma, atualiza o também arquivo de versões ```k_version.hpp``` com informações de build.

> **Nota**: o script executa um programa _Lua_ (```build/auto-increment.lua```) para atualizar o arquivo de versão ```k_version.hpp```. O projeto também disponibiliza o interpretador **Lua** para ambientes de desenvolvimento Microsoft Windows e Linux.

## Processo de Building com Versionamento Automático

Este projeto utiliza sistema de versão baseado em [**Semantic Version**](https://semver.org/).

  ```
    MAJOR.MINOR.PATCH
  ```

Além disso, como é um projeto multiplataforma, também foi necessário incluir informações de build na versão para facilitar o processo de testes e depuração:

```
  MAJOR.MINOR.PATCH - GIT_HASH - BUILD_TYPE - BUILD_NUMBER - BUILD_DATE - PLATFORM
```

- GIT_HASH é o hash do commit para identificar o commit referente a versão que está sendo executado
- BUILD_TYPE é o tipo de build: Release ou Debug
- BUILD_NUMBER é o número de incremento utilizado somente em versões de publicação
- BUILD_DATE é a data de build
- PLATFORM indica se foi compilado para o sistema operacional Microsoft Windows, Linux ou Android

Os scripts (descritos no tópico anterior) foram criados para facilitar a obtenção dessas informações de build e incluí-los no arquivo de header ```k_version.hpp```.

```
  // (!) the lines below are automatically updated by external building tool
  #define K_ENGINE_GIT_HASHCODE 0
  #define K_ENGINE_BUILD_TYPE 2
  #define K_ENGINE_VERSION_BUILD_INCREMENT 0
  #define K_ENGINE_BUILD_DATE 0
  #define K_ENGINE_BUILD_PLATFORM 2
```

## Etapas de building [em construção]

Ideias iniciais de pipeline de building para incrementar a versão automaticamente e também para facilitar a integração de CI no futuro:

1. Desenvolvimento/build local
2. Teste
3. Executar a ferramenta externa de building
  - auto increment
  - commit (git)
  - pull request

Este processo de building gera a nova versão no arquivo ```engine/include/k_version.hpp```.

- https://semver.org/
- https://stackoverflow.com/questions/5007707/versioning-with-an-automatic-build-system
- https://stackoverflow.com/questions/7724569/debug-vs-release-in-cmake
- https://stackoverflow.com/questions/51727566/how-to-make-git-commit-hash-available-in-c-code-without-needless-recompiling

## Biblioteca estática vs dinâmica [em construção]

A distribuição de *K-Engine* está na forma de uma biblioteca estática ```kengine.lib``` e um conjunto de arquivos headers (```core/inc/*.hpp```). Toda a estrutura de diretórios foi pensada para facilitar a compilação e obter facilmente os arquivos necessários para incluí-los em outros projetos.

Estudar a possibilidade de geração da biblioteca na forma dinâmica (dll ou .so)

## Demonstração, exemplos e testes [em construção]

O projeto também gera um executável chamado ```game``` que é o exemplo principal que demonstra as funcionalidades da versão atual.

A estrutura conta também com diretórios separados com exemplos de uso, testes, avaliação de desempenho e documentação.

[imagem da estrutura de diretórios]

Arquivos necessários para utilizar k-engine em seus projetos:

- ```kengine.lib```
- arquivos headers (.hpp) em ```core/inc/```
