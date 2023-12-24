#include "Rectangle.h"
namespace pertyG
{
    Rectangle::Rectangle() : Rectangle(Point(0, 0), 0, 0)
    {

    }
    Rectangle::Rectangle(Point position, double width, double height)
        : mPosition(position)
    {
        mSize[0].initValue(width);
        mSize[1].initValue(height);
    }

    // Getter methods

    Point& Rectangle::getPosition()
    {
        return mPosition;
    }

    Point Rectangle::getCorner(int cornerID)
    { 
        switch (cornerID) 
        {
        case TopLeft:
            return mPosition;
        case TopRight:
            return mPosition.withOffset(Point(getWidth(), 0.0));
        case BottomRight:
            return mPosition.withOffset(Point(getWidth(), getHeight()));
        case BottomLeft:
            return mPosition.withOffset(Point(0.0, getHeight()));
        }
        
    }
    Property& Rectangle::getWidth() { return mSize[0]; }
    Property& Rectangle::getHeight() { return mSize[1]; }

    Rectangle Rectangle::withSizeKeepCenter(double newWidth, double newHeight)
    {
        Rectangle newRectangle;
        newRectangle = *this;
        newRectangle.moveBy(Point((mSize[0] - newWidth) / 2, (mSize[1] - newHeight) / 2));
        newRectangle.setSize(newWidth, newHeight);
        return newRectangle;
    }

    // Setter methods

    void Rectangle::setPosition(const Point& position) { mPosition = position; }
    void Rectangle::setSize(double width, double height)
    {
        mSize[0].setValue(width);
        mSize[1].setValue(height);
    }

    // Move the rectangle by a certain offset

    void Rectangle::moveBy(const Point& vector) { mPosition; }

    // Resize the rectangle by a certain factor

    void Rectangle::resizeBy(double widthFactor, double heightFactor)
    {
        mSize[0].setValue(mSize[0] * widthFactor);
        mSize[1].setValue(mSize[1] * heightFactor);
    }
}