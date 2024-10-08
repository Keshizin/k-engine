#
# CMakeLists.txt for DEMO directory
#

file (GLOB SOURCE "*.cpp")

add_executable(${APPNAME} ${SOURCE})

set_target_properties(${APPNAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

if(UNIX)
	target_link_libraries(${APPNAME} ${LIBNAME} X11 GL GLU)
else()
	target_link_libraries(${APPNAME} ${LIBNAME} opengl32)
endif()

target_include_directories(${APPNAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
	"${PROJECT_SOURCE_DIR}/demo"
)
