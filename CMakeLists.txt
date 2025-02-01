#
# CMakeLists.txt for K-Engine Project!
#

# The minimum version of CMake must be 3.21 to support own built-in Android NDK.
#	https://developer.android.com/ndk/guides/cmake
cmake_minimum_required(VERSION 3.21)
set(CMAKE_VERBOSE_MAKEFILE on)

#
#  project name
#	https://cmake.org/cmake/help/latest/command/project.html#command:project
#
project(k-engine)
enable_language(CXX)

#
#  output filename
#
set(APPNAME game)
set(LIBNAME kengine)
set(KENGINE_TEST_NAME kengine_test)

#
# output binary directory
#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#
# compiler options
#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)

if(MSVC)
	# C4100 - the formal parameter is not referenced in the body of the function. The unreferenced parameter is ignored.
	# c4710 - the given function was selected for inline expansion, but the compiler did not perform the inlining.
	# c4711 - the compiler performed inlining on the given function, although it was not marked for inlining.
	# C4820 - the type and order of elements caused the compiler to add padding to the end of a struct.
	# C5045 - Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

	set(WARNING_FLAGS "/EHs /wd4100 /wd4710 /wd4711 /wd4820 /wd5045")
endif(MSVC)

set(CMAKE_CXX_FLAGS "-Wall ${WARNING_FLAGS}")

#
# sub directories
#
add_subdirectory(engine)
add_subdirectory(demo)

#
# test directories
#

#if(KENGINE_TEST_UNIT)
enable_testing()
add_subdirectory(tests)
#ndif()