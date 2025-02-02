#
# CMakeLists.txt for third/imgui directory
#

file (GLOB SOURCE "*.cpp")
file (GLOB BACKENDS_SOURCE "backends/*.cpp")

add_library(imgui STATIC ${SOURCE} ${BACKENDS_SOURCE})

target_include_directories(imgui PUBLIC
	"${PROJECT_SOURCE_DIR}/third/imgui-1.91.8"
)

if(MSVC)
	target_compile_options(imgui PRIVATE /w) 
endif(MSVC)