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
    Rectangle Rectangle::operator=(const Rectangle& other)
    {
        mSize[0] = other.mSize[0];
        mSize[1] = other.mSize[1];
        mPosition = other.mPosition;
        return *this;
    }
    Rectangle::Rectangle(const Rectangle& other) : mPosition(other.mPosition)
    {
        mSize[0] = other.mSize[0];
        mSize[1] = other.mSize[1];
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

    Rectangle Rectangle::withPosition(Point position)
    {
        Rectangle newRectangle = *this;
        newRectangle.setPosition(position);
        return newRectangle;
    }

    bool Rectangle::isInside(Point point)
    {
        if (point.getX() > getCorner(TopLeft).getX() 
            && point.getY() > getCorner(TopLeft).getY()
            && point.getX() < getCorner(BottomRight).getX()
            && point.getY() < getCorner(BottomRight).getY())
            return true;
        return false;
    }

    // Setter methods

    void Rectangle::setPosition(const Point& position) { mPosition = position; }
    void Rectangle::setSize(double width, double height)
    {
        mSize[0].setValue(width);
        mSize[1].setValue(height);
    }

    // Move the rectangle by a certain offset

    void Rectangle::moveBy(Point vector)
    {
        mPosition.move(vector);
    }

    // Resize the rectangle by a certain factor

    void Rectangle::resizeBy(double widthFactor, double heightFactor)
    {
        mSize[0].setValue(mSize[0] * widthFactor);
        mSize[1].setValue(mSize[1] * heightFactor);
    }
}