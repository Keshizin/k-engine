#
# CMakeLists.txt for K-Engine Project!
#

cmake_minimum_required(VERSION 3.17)
set (CMAKE_CXX_STANDARD 11)
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
# sub directories
#
add_subdirectory(engine)
add_subdirectory(demo)
