#
# CMakeLists.txt for DEMO directory
#

file (GLOB SOURCE "*.cpp")

add_executable(${APPNAME} ${SOURCE})

target_link_libraries(${APPNAME} ${LIBNAME})

target_include_directories(${APPNAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)
