#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <iostream>

const char* vertexShaderSource = R"(
    #version 460 core

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec2 vertexPosition_modelspace;
    layout(location = 1) in vec4 vertexColor;
	layout (location = 2) uniform mat4 projection;

    // Output data ; will be interpolated for each fragment.
    out vec4 fragmentColor;
    // Values that stay constant for the whole mesh.

    void main(){	

         gl_Position = projection * vec4(vertexPosition_modelspace, 0.0, 1.0);
        gl_Position.y = -gl_Position.y;
        //gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);
	    // The color of each vertex will be interpolated
	    // to produce the color of each fragment
	    fragmentColor = vertexColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core

    // Interpolated values from the vertex shaders
    in vec4 fragmentColor;

    // Ouput data
    out vec4 color;

    void main(){

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;

    }
)";
GLuint shaderProgram;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Square", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glewExperimental = GL_TRUE;
    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set the callback for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile and link the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Set up the vertex data and buffers
    float vertices[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        300, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        300, 600.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 600.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };


    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Projection matrix uniform
    GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
            glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        });
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the square
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
