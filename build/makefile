# -----------------------------------------------------------------------------
#  K-Engine MAKEFILE for Visual Studio C++ Compiler
# -----------------------------------------------------------------------------

# Caminho dos diretórios
CORE_INC_DIR=../core/inc
CORE_SRC_DIR=../core/src
LIBNAME=kengine.lib

# Compilation and Linking Flags
SRC_LIST=$(wildcard $(CORE_SRC_DIR)/*.cpp)
OBJ_LIST=$(patsubst $(CORE_SRC_DIR)/%.cpp,%.obj,$(SRC_LIST))
MSVCFLAGS=/EHsc /I$(CORE_INC_DIR)
LIBFLAGS=/link $(LIBNAME) gdi32.lib opengl32.lib glu32.lib user32.lib

# # Game Executable Generation
all: $(LIBNAME)
	CL ../game/main.cpp $(MSVCFLAGS) $(LIBFLAGS) /OUT:game.exe

# K-Engine Static Library Generation
# $(LIBNAME): $(SRC_LIST:.cpp=.obj)
$(LIBNAME): $(OBJ_LIST)
	lib $^ /OUT:$@

%.obj: $(CORE_SRC_DIR)/%.cpp
	CL /c $< $(MSVCFLAGS)

clean:
	del *.lib
	del *.obj
	del *.exe

rebuild:
	$(MAKE) clean
	$(MAKE)
