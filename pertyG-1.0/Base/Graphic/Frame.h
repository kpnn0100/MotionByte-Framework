#pragma once
#include <GLFW/glfw3.h>
#include "Color.h"
namespace pertyG
{
    class Frame {
    private:
        GLFWwindow* mainWindow;

    public:
        // Constructor that takes a GLFWwindow*
        Frame(GLFWwindow* window) : mainWindow(window) {}

        // Method to fill color in the buffer
        void fillColor(Color color);

        // Method to draw something to the buffer (you can replace this with your drawing logic)
        void drawSomething();

        // Method to swap the buffers and show the content
        void showBuffer();
    };
}