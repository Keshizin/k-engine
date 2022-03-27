# -----------------------------------------------------------------------------
#  K-Engine MAKEFILE for Visual Studio C++ Compiler
# -----------------------------------------------------------------------------

# relative directory path
CORE_INC_DIR=../core/inc
CORE_SRC_DIR=../core/src

# output name
LIBNAME=kengine.lib
GAMENAME=game.exe

# compilation and linking flags
SRC_LIST=$(wildcard $(CORE_SRC_DIR)/*.cpp)
OBJ_LIST=$(patsubst $(CORE_SRC_DIR)/%.cpp,%.obj,$(SRC_LIST))
MSVCFLAGS=/EHsc /I$(CORE_INC_DIR) /Wall
LIBFLAGS=/link $(LIBNAME) gdi32.lib opengl32.lib glu32.lib user32.lib

# game executable eeneration
all: $(LIBNAME)
	CL ../game/main.cpp $(MSVCFLAGS) $(LIBFLAGS) /OUT:$(GAMENAME)
	@del *.obj
	@rmdir /s /q out 
	@mkdir out
	@mkdir out\game
	@mkdir out\lib
	@move $(GAMENAME) "out/game"
	@move $(LIBNAME) "out/lib"


# K-Engine Static Library Generation
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
