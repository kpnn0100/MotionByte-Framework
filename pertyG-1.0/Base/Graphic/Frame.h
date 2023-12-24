#pragma once
#include <memory>

#include "../Window/GlwfManager.h"
#include "Color.h"
#include "../Segment/Rectangle.h"
namespace pertyG
{
    class Frame {
    private:
        static int windowWidth, windowHeight;
        Rectangle* mBound;
    public:
        // Constructor that takes a std::shared_ptr<GLFWwindow>
        Frame(Rectangle* bound);
        // Method to fill color in the buffer
        static void onWindowSizeChanged(GLFWwindow* window, int width, int height);
        void fillColor(Color color);
        void drawCircle(Color color, Rectangle bound, double stroke);
        void drawRectangle(Color color, Rectangle bound, double stroke);
        // Method to draw something to the buffer (you can replace this with your drawing logic)
        void drawSomething();

    };
}