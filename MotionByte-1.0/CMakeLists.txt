find_package(OpenGL REQUIRED)
### GLFW ###
add_subdirectory(SDK)
# Check if the target operating system is Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux-specific code here
    message("Configuring for Linux")
    
    set(ALL_LIBS
	    ${OPENGL_LIBRARY}
        freetype
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
        freetype
    )
# Default case for unsupported operating systems
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()
    
file(GLOB_RECURSE DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Base/**/*.h")


include_directories(
	SDK/glfw-3.1.2/include/
	SDK/glm-0.9.7.1/
	SDK/glew-1.13.0/include/
    Base
    SDK/freetype-2.9/include
	.
)

file(GLOB_RECURSE MOTION_BYTE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Base/**/*.cpp")
list(APPEND MOTION_BYTE_SOURCE)
add_library(MotionByte ${MOTION_BYTE_SOURCE})
message("Source files: ${ALL_LIBS}")
message("Header: ${DIRS}")
target_link_libraries(MotionByte PRIVATE ${ALL_LIBS})
target_include_directories(MotionByte PUBLIC SDK/glfw-3.1.2/include/
	SDK/glm-0.9.7.1/
	SDK/glew-1.13.0/include/
    Base
    SDK/freetype-2.9/include
	.)