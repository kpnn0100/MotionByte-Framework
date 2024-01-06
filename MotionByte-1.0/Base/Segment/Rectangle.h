#pragma once
#include "Point.h"
#include "../Property/Property.h"

namespace MotionByte
{
    class Rectangle
    {
        Point mPosition;
        Property mSize[2]; // Assuming mSize is a 2-element array for width and height.

    public:
        enum Corner
        {
            TopLeft = 0,
            TopRight,
            BottomRight,
            BottomLeft,
            CornerCount
        };
        Rectangle();
        Rectangle(Point position, double width, double height);
        Rectangle(const Rectangle& other);
        Rectangle operator=(const Rectangle& other);
        // Getter methods
        Point& getPosition();
        Point getCorner(int cornerID);
        Point getCenter();
        Property& getWidth();
        Property& getHeight();
        Rectangle withSizeKeepCenter(double newWidth,double newHeight);
        Rectangle withPosition(Point position);
        
        bool isInside(Point point);
        // Setter methods
        void setPosition(const Point& position);
        void setSize(double width, double height);

        // Move the rectangle by a certain offset
        void moveBy(Point vector);

        // Resize the rectangle by a certain factor
        void resizeBy(double widthFactor, double heightFactor);
    };
}
