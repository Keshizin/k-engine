#
# CMakeLists.txt for K-Engine Project!
#

cmake_minimum_required(VERSION 3.17)
set(CMAKE_VERBOSE_MAKEFILE on)


#
#  project name
#
project(k-engine)


#
#  output filename
#
set(APPNAME game)
set(LIBNAME kengine)


#
# including path for include
#
include_directories("${PROJECT_SOURCE_DIR}/game")


#
# setting compilation and linker flags
#

# MSVC is a cmake-variable that set to true when the compiler is some version of Microsoft Visual C++
if(MSVC)
	set (CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHs -Wall /wd4514 /wd4710 /wd4711 /wd5045 /wd4820 /wd4668")
endif(MSVC)

add_definitions(-DK_DEBUG)


#
# setting sub directory
#
add_subdirectory(core)
add_subdirectory(game)
