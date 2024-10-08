#
# CMakeLists.txt for K-Engine directory
#

file (GLOB SOURCE "*.cpp")

add_library(${LIBNAME} STATIC ${SOURCE})

target_compile_definitions(${LIBNAME} PUBLIC K_ENGINE_DEBUG)

target_include_directories(${LIBNAME} PUBLIC
	include
	include/GL
)
