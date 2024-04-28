#
# CMakeLists.txt for K-Engine Project!
#
cmake_minimum_required(VERSION 3.17)

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
	# C4820 - the type and order of elements caused the compiler to add padding to the end of a struct.
	# C5045 - Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
	# c4710 - the given function was selected for inline expansion, but the compiler did not perform the inlining.
	# c4711 - the compiler performed inlining on the given function, although it was not marked for inlining.

	set(WARNING_FLAGS "/EHs /wd4100 /wd4710 /wd4711 /wd4820 /wd5045")
endif(MSVC)

set(CMAKE_CXX_FLAGS "-Wall ${WARNING_FLAGS}")

#
# sub directories
#
add_subdirectory(engine)
add_subdirectory(demo)