[![Build Status](https://www.travis-ci.com/Keshizin/k-engine.svg?branch=main)](https://www.travis-ci.com/Keshizin/k-engine)

<img src="_docs/img/logo.png" alt="K-Engine Logo" align="right" width=200 />

# K-Engine

K-Engine is a open source game engine written in C++ developed for my undergraduate thesis in Computer Science.

The main motivation for this project is to study game engine technologies and architectures, as well as understand other fields of game development and computer science in a deeper way.

All design ideas and references used in this project can be found in `_docs`.

K-Engine has the following features:

- Support to Microsoft Windows platform
- Rendering System based on OpenGL 4.5
- According to the C++11 standard

**Note**: This project does not use third party libraries for windowing system like SDL, GLFW, etc.

Full specification and documentation can be found in `_docs`.


## Getting Starting

This project use `cmake` to generate makefile for these compilers:

- MSVC 16 2019 x64

```
git clone https://github.com/Keshizin/k-engine.git
cd k-engine/build
build-msvc-x64.bat
```

This will generate static library `kengine.lib` that you can use to link to your game project. For more details, check the development guide (inside the `_docs` directory) to find out how to create games using K-Engine.

## License

K-Engine is licensed under the MIT License.
