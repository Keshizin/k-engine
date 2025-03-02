#
# CMakeLists.txt for TESTS directory
#

# include(FetchContent)

# FetchContent_Declare(
#    Catch2
#    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#    GIT_TAG v3.8.0
# )

# FetchContent_MakeAvailable(Catch2)

add_executable(MESH_TEST "mesh_test.cpp")
add_executable(MATH_TEST "math_test.cpp")

#target_link_libraries(${KENGINE_TEST_NAME} PRIVATE Catch2::Catch2WithMain ${LIBNAME})
target_link_libraries(MESH_TEST PRIVATE ${LIBNAME})
target_link_libraries(MATH_TEST PRIVATE ${LIBNAME})

target_include_directories(MESH_TEST PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)

target_include_directories(MATH_TEST PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)

add_test(NAME KENGINE_MESH_TEST COMMAND MESH_TEST)
add_test(NAME KENGINE_MATH_TEST COMMAND MATH_TEST)