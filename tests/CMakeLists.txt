#
# CMakeLists.txt for TESTS directory
#

add_executable(${KENGINE_TEST_NAME} main_tests.cpp)

target_link_libraries(${KENGINE_TEST_NAME} ${LIBNAME})

target_include_directories(${KENGINE_TEST_NAME} PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/include"
)

add_test(NAME KENGINE_TEST COMMAND ${KENGINE_TEST_NAME})