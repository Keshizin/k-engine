#
# CMakeLists.txt for K-Engine directory
#

file (GLOB SOURCE "*.cpp")

add_library(${LIBNAME} STATIC ${SOURCE})

target_include_directories(${LIBNAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)
