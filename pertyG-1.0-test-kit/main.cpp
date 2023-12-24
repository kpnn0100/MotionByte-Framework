#include "../pertyG-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pertyG;

void submain(Window * window) {
    while (true)
    {
        int num1, num2;
        std::cout << "Enter two integers: " << std::endl;
        std::cin >> num1 >> num2;
        std::cout << std::endl;
        window->setSize(num1, num2);
    }
}

int main() {
    Window window(500, 600);
    window.getPropertyManager().getProperty(Window::WindowWidth).setInterpolator(InterpolatorFactory::createLinear(2.0));
    window.getPropertyManager().getProperty(Window::WindowHeight).setInterpolator(InterpolatorFactory::createLinear(2.0));
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    

    window.waitToClose();
    submainThread.join();
    return 0;
}
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <vector>
//
//// Vertex and Fragment shaders
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec2 position;
//
//    void main()
//    {
//        gl_Position = vec4(position, 0.0, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 color;
//
//    void main()
//    {
//        color = vec4(1.0, 1.0, 1.0, 1.0);
//    }
//)";
//
//GLFWwindow* window;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//int main(void)
//{
//    if (!glfwInit())
//    {
//        fprintf(stderr, "Failed to initialize GLFW\n");
//        getchar();
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    window = glfwCreateWindow(1024, 768, "Draw Circle", NULL, NULL);
//    if (window == NULL) {
//        fprintf(stderr, "Failed to open GLFW window.\n");
//        getchar();
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    glewExperimental = true;
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        getchar();
//        glfwTerminate();
//        return -1;
//    }
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glUseProgram(shaderProgram);
//
//    GLuint vertexArrayID;
//    glGenVertexArrays(1, &vertexArrayID);
//    glBindVertexArray(vertexArrayID);
//
//    GLuint vertexBuffer;
//    glGenBuffers(1, &vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//
//        std::vector<float> vertices;
//        const float radius = 0.5f;
//        const int segments = 10;
//
//        for (int i = 0; i <= segments; ++i) {
//            float theta = 2.0f * 3.14159265 * static_cast<float>(i) / static_cast<float>(segments);
//            float x = radius * glm::cos(theta);
//            float y = radius * glm::sin(theta);
//            vertices.push_back(x);
//            vertices.push_back(y);
//        }
//
//        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    
//
//    while (!glfwWindowShouldClose(window))
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//        glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 2);
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &vertexArrayID);
//    glDeleteBuffers(1, &vertexBuffer);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//
//    return 0;
//}

