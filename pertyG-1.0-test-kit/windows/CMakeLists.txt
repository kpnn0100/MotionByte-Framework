cmake_minimum_required(VERSION 3.0)
project(WindowsTestProject)
include_directories("../../SDK/glfw-3.3.8.bin.WIN64/include")
# Gather a list of all .cpp files in the "pertyG-1.0" directory recursively
file(GLOB_RECURSE PERTYG_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../pertyG-1.0/**/*.cpp")
message("Source files: ${PERTYG_SOURCE_FILES}")

# Create the executable
add_executable(windows_test "../main.cpp" ${PERTYG_SOURCE_FILES})
#target_link_libraries(windows_test PRIVATE "G:/Dev/pertyG framework/SDK/glfw-3.3.8.bin.WIN64/lib-vc2017/glfw3.lib")
target_link_libraries(windows_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../SDK/glfw-3.3.8.bin.WIN64/lib-vc2017/glfw3.lib")