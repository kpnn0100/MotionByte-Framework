cmake_minimum_required(VERSION 2.6)
find_package(OpenGL REQUIRED)
### GLFW ###
add_subdirectory(SDK)
# Check if the target operating system is Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux-specific code here
    message("Configuring for Linux")
    
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
    
# Check if the target operating system is Windows
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows-specific code here
    message("Configuring for Windows")
    set(ALL_LIBS
	    ${OPENGL_LIBRARY}
	    glfw
        GLEW_1130
    )
# Default case for unsupported operating systems
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()
    

include_directories(
	SDK/glfw-3.1.2/include/
	SDK/glm-0.9.7.1/
	SDK/glew-1.13.0/include/
	MotionByte-1.0/
	.
)

#this is working
file(GLOB_RECURSE PERTYG_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/MotionByte-1.0/**/*.cpp")
list(APPEND PERTYG_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/SDK/glew-1.13.0/src/glew.c")
add_library(MotionByte SHARED ${PERTYG_SOURCE_FILES})
target_link_libraries(MotionByte PUBLIC ${ALL_LIBS})
message("Source files: ${PERTYG_SOURCE_FILES}")
add_executable(windows_test ${PERTYG_SOURCE_FILES} "MotionByte-test-kit/main.cpp")
target_link_libraries(windows_test PUBLIC ${ALL_LIBS})