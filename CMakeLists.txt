cmake_minimum_required(VERSION 2.6)
add_subdirectory(MotionByte-1.0)
add_executable(windows_test "MotionByte-test-kit/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/MotionByte-1.0/SDK/glew-1.13.0/src/glew.c")

target_link_libraries(windows_test PRIVATE MotionByte)