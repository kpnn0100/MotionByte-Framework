#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable anti-aliasing and subpixel rendering
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw a simple line
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glVertex2f(100.5f, 100.5f); // Start at half-pixel position
    glVertex2f(200.5f, 200.5f); // End at half-pixel position
    glEnd();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Subpixel Rendering Example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        drawScene();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
