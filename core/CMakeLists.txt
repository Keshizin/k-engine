#
# CMakeLists.txt for core directory
#

file (GLOB SOURCE "src/*.cpp")

#source_group ("inc" FILES ${HEADER})
#source_group ("src" FILES ${SOURCE})

add_definitions(-DK_DEBUG)

add_library(${LIBNAME} STATIC ${SOURCE})

target_include_directories(${LIBNAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/core/inc"
)