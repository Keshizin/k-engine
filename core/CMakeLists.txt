file (GLOB HEADER "inc/*.h")
file (GLOB SOURCE "src/*.cpp")

source_group ("inc" FILES ${HEADER})
source_group ("src" FILES ${SOURCE})

include_directories(
	"${PROJECT_SOURCE_DIR}/core/inc"
	"${PROJECT_SOURCE_DIR}/core/src")

#add_definitions(-DK_DEBUG)

add_library(kengine STATIC ${SOURCE})