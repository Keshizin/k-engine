# Building K-Engine!

Pre-requisites:

- MSVC 2019 or greater for MS Windows
- GCC compiler for Linux
- CMake
- Lua (binaries are included with this project)

## Building System

O projeto K-Engine possui o seu próprio **CMAKE** para as plataformas **Microsoft Windows** e **Linux** (e, futuramente, para **Android**) que permite realizar configurações específicas do projeto.

Para cada plataforma existem scripts para realizar build do projeto.

- ```build/win/win-build.bat [Release or Debug]```
- ```build/linux/linux-build.sh [Release or Debug]```

Estes scripts, além de executar o comando **cmake** e **make** para cada plataforma, atualiza também o arquivo de versões ```k_version.hpp``` com informações de build.

> **Nota**: o script executa um programa _Lua_ (```build/auto-increment.lua```) para atualizar o arquivo de versão ```k_version.hpp```. O projeto também disponibiliza o interpretador **Lua** para ambientes de desenvolvimento Microsoft Windows e Linux.

## Processo de Building com versionamento automático

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

Este processo de building gera uma nova versão no arquivo ```engine/include/k_version.hpp```.

Referências de estudo:

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

## Compiler Warnings from MSVC

Dependendo do compilador, é possível que diversos warnings sejam gerados durante a compilação. Este tópico contém informações a respeito destes warnings.

Apesar de não ser necessário incluir nomes de parâmetros em declarações de funções. É melhor colocá-los para questão de legibilidade. Imagina que o usuario vai consultar o arquivo header e ele consegue identificar o significado de cada parametro pelo seu nome.

A configuração padrão de CMAKE para MSVC 2019 estava habilitando a flag -EHsc que corresponde ao modelo de tratamento de exceções em MVSC.

Segundo a documentação da Microsoft (MSDN), a opção "c" faz com que o compilador assuma que as funções declaradas como "extern C" nunca irão lançar uma exceção.

Com a configuração padrão, o compilador estava exibindo warning sobre a função TpSetCallbackCleanupGroup que poderia ser lançada uma exceção:

warning C5039: 'TpSetCallbackCleanupGroup': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.

Como correção, a opção "c" foi desabilitada no CMAKE para a MSVC:

set (CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHs -Wall")

Para o compilador MSVC foi necessário desativar alguns tipos de warnings. Segue a lista de significado de cada warning desabilitada:

Compiler Warning (level 4) C4514 - 'function' : unreferenced inline function has been removed.
Compiler Warning (level 4) C4710 - the given function was selected for inline expansion, but the compiler did not perform the inlining.
Compiler Warning (level 1) C4711 - the compiler performed inlining on the given function, although it was not marked for inlining.
Compiler Warning (level 4) C4820 - the type and order of elements caused the compiler to add padding to the end of a struct.
Fontes das referências:
https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4514?view=msvc-160
https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4710?view=msvc-160
https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4711?view=msvc-160
https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4820?view=msvc-160

Nota: É necessário uma forma de realizar essa desabilitação no CMAKE. Atualmente está sendo editada diretamente na flag CMAKE_CXX_FLAGS como por exemplo usar target_compile_options ou pragmas dentro do código.

## Como compilar no Android (Android NDK) - EM CONSTRUÇÃO
