#
# CMakeLists.txt for game directory
#

file (GLOB SOURCE "*.cpp")

add_executable(${APPNAME} WIN32 ${SOURCE})

target_link_libraries(${APPNAME} ${LIBNAME} opengl32 glu32)

target_include_directories(${APPNAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/core/inc"
)