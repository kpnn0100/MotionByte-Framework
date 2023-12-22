cmake_minimum_required(VERSION 2.6)
find_package(OpenGL REQUIRED)
### GLFW ###
add_subdirectory(SDK)
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	X11
    Xrandr
    Xinerama
    pthread
    dl
	Xcursor
)
include_directories(
	SDK/glfw-3.1.2/include/
	SDK/glm-0.9.7.1/
	SDK/glew-1.13.0/include/
	pertyG-1.0/
	.
)

#this is working
file(GLOB_RECURSE PERTYG_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pertyG-1.0/**/*.cpp")
list(APPEND PERTYG_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/SDK/glew-1.13.0/src/glew.c")
add_library(pertyG STATIC ${PERTYG_SOURCE_FILES})
target_link_libraries(pertyG PUBLIC ${ALL_LIBS})
message("Source files: ${PERTYG_SOURCE_FILES}")
add_executable(windows_test "pertyG-1.0-test-kit/main.cpp")
target_link_libraries(windows_test PUBLIC pertyG)