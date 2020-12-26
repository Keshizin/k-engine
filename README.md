<img src="_docs/img/logo.png" alt="K-Engine Logo" align="right" width=100 />

# K-Engine

K-Engine is a open source game engine written in C++ developed for my undergraduate thesis in Computer Science.

## Getting Starting

This project use cmake to generate makefile for these compilers:

- MSVC 2019 x64
- MinGW x32

1. Go to https://github.com/Keshizin/k-engine to clone K-Engine repo.

         $ git clone https://github.com/Keshizin/k-engine.git

2. After cloning the repo, go to the build directory and run the script corresponding to your compiler.

         $ cd build
         $ build-msvc-x64.bat

This will generate static library `kengine.lib` that you can use to link to your game project and a game executable. For more details, check the development guide (inside the `_docs` directory) to find out how to create games using K-Engine.

## License

K-Engine is licensed under the MIT License.
