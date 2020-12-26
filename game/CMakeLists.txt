include_directories("${PROJECT_SOURCE_DIR}/core/inc")

# add_executable(${APP_EXE} WIN32 main.cpp)
add_executable(${APP_EXE} main.cpp)
target_link_libraries(${APP_EXE} kengine)