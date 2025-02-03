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

add_executable(${KENGINE_TEST_NAME} "mesh_test.cpp")

#target_link_libraries(${KENGINE_TEST_NAME} PRIVATE Catch2::Catch2WithMain ${LIBNAME})
target_link_libraries(${KENGINE_TEST_NAME} PRIVATE ${LIBNAME})

target_include_directories(${KENGINE_TEST_NAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)

add_test(NAME KENGINE_MESH_TEST COMMAND ${KENGINE_TEST_NAME})