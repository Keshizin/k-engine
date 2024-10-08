#
# CMakeLists.txt for K-Engine directory
#

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# o header "<android_native_app_glue.h>" fica visivel no projeto
include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

file (GLOB SOURCE "*.cpp")

add_library(${LIBNAME} STATIC ${SOURCE})
target_compile_definitions(${LIBNAME} PUBLIC K_ENGINE_DEBUG)

set_target_properties(${LIBNAME}
		PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIBNAME},INCLUDE_DIRECTORIES>
)

target_include_directories(${LIBNAME} PUBLIC
	include
	include/GL
)

target_link_libraries(${LIBNAME}
		PUBLIC
		native_app_glue
		GLESv2
		EGL
		android
		log
)
