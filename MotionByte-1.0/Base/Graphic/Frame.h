#pragma once
#include <memory>

#include "../Window/GlwfManager.h"
#include "Color.h"
#include "../Segment/Rectangle.h"

namespace MotionByte
{
    class Window;
    class Frame {
    private:
        static int windowWidth, windowHeight;
        Window* mWindow;
        Rectangle* mBound;
    public:
        enum Direction
        {
            ClockWise,
            CounterClockWise
        };
        // Constructor that takes a std::shared_ptr<GLFWwindow>
        Frame(Rectangle* bound);
        void setWindow(Window* window);
        // Method to fill color in the buffer
        static void onWindowSizeChanged(GLFWwindow* window, int width, int height);
        void fillColor(Color color);
        void drawRectangle(Color color, Rectangle bound, double stroke);
        void fillRectangle(Color color, Rectangle bound);
        void drawRoundedRectangle(Color color, Rectangle bound, double radius, double stroke);
        void fillRoundedRectangle(Color color, Rectangle bound, double radius);
        void drawCircle(Color color, Rectangle bound, double stroke);
        void fillCircle(Color color, Rectangle bound);
        void drawArc(Color color, Rectangle bound, double stroke, double startDegree, double endDegree, Direction direction);
        void drawArc(Color color, Point center, double radius, double stroke, double startDegree, double endDegree, Direction direction);
        void drawAnnularArc(Color color, Point center, double innerRadius,double outerRadius, double startDegree, double endDegree, Direction direction);
        void drawSomething();

    };
}