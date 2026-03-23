# K-Engine

**K-Engine** is a open source game engine written in C++ developed for my undergraduate thesis in Computer Science (2020).

The main motivation for this project is to study game engine architectures and its technologies, as well as understand other fields of game development and computer science in a deeper way.

Platforms supported:

- Microsoft Windows (Win32 API)  
- Linux (XLib)  
- Android (Android NDK)  

Graphics API supported:

- OpenGL (WGL, GLX and EGL)

## Building the project

How to build for MSVC:

```
  > cd build/win
  > win-build.bat [Release or Debug]
```

How to build for GCC:

```
  $ cd build/linux
  $ linux-build.sh [Release or Debug]
```

For more details, check [Building](_docs/building.md).

## License

K-Engine is licensed under the MIT License.

## References

OpenGL:
- WOLFF, D. OpenGL 4 Shading Language Cookbook. 3rd ed. Birmingham: Packt Publishing, 2018.  
- GORDON, V. S. CLEVENGER, J. Computer Graphics Programming in OpenGL with C++, 3rd ed. Mercury Learning and Information, 2024.  
- KESSENICH, J. et al. OpenGL Programming Guide: the official guide to learning OpenGL, versions 4.5. 9th ed. Boston: Pearson Education, 2017.  
- COHEN, M. MANSSOUR, I. OpenGL Uma Abordagem Prática e Objetiva. Novatec, 2006.

Game Development:
- GREGORY, J. Game Engine Architecture. 3rd ed. Boca Raton: Taylor & Francis, CRC Press, 2018.
- PENAZZO, D. 2D Game Development: From Zero To Hero. Avaible in [https://therealpenaz91.itch.io/2dgd-f0th](https://therealpenaz91.itch.io/2dgd-f0th).
- PERUCIA, A. et al. Desenvolvimento de Jogos Eletrônicos: teoria e prática, 2a edição. São Paulo: Novatec Editora, 2007.
- MORRISON, M. Game Programming in 24 hours. Sams Publishing, 2003.  

Physics Engine:
- MILLINGTON, I. Game Physics Engine Development, 2nd ed. Morgan Kaufmann Publishers, 2010.

Computer Graphics:
- Scratch a pixel. Available in [https://www.scratchapixel.com/](https://www.scratchapixel.com/)  

C++:
- STROUSTRUP, B. The C++ programming language, 4th ed. Reading, MA: Addison-Wesley, 2013.
