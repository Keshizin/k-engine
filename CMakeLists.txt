cmake_minimum_required(VERSION 3.17)
set(CMAKE_VERBOSE_MAKEFILE on)

# -----------------------------------------------------------------------------
#  P R O J E C T   N A M E
# -----------------------------------------------------------------------------
project(k-engine)

# -----------------------------------------------------------------------------
#  G A M E   N A M E
# -----------------------------------------------------------------------------
set(APP_EXE game)

# -----------------------------------------------------------------------------
#  S E T T I N G   C O M P I L A T I O N   &   L I N K E R   F L A G S
# -----------------------------------------------------------------------------

# MSVC is a cmake-variable that set to true when the compiler is some version of Microsoft Visual C++
if(MSVC)
	set (CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHs -Wall /wd4514 /wd4710 /wd4711 /wd5045 /wd4820 /wd4668")
endif(MSVC)

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

# -----------------------------------------------------------------------------
#  S E T T I N G   S U B   D I R E C T O R Y
# -----------------------------------------------------------------------------
add_subdirectory(core)
add_subdirectory(game)
