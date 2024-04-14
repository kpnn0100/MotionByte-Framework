#include "Rectangle.h"
namespace MotionByte
{
    Rectangle::Rectangle() : Rectangle(Point(0, 0), 0, 0)
    {

    }
    Rectangle::Rectangle(double x1, double y1, double x2, double y2) :
        Rectangle(Point(x1, y1), x2 - x1, y2 - y1)
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
    Point Rectangle::getCenter()
    {
        Point center = mPosition;
        center = center.withOffset(Point(mSize[0] / 2.0, mSize[1] / 2.0));
        return center;
    }
    Point Rectangle::getLocalCenter()
    {
        Point center;
        center = center.withOffset(Point(mSize[0] / 2.0, mSize[1] / 2.0));
        return center;
    }
    Property& Rectangle::getWidth() { return mSize[0]; }
    Property& Rectangle::getHeight() { return mSize[1]; }

    Rectangle Rectangle::withSizeKeepCenter(double newWidth, double newHeight)
    {
        Rectangle newRectangle;
        newRectangle = *this;
        newRectangle.moveBy(Point((mSize[0] - newWidth) / 2, (mSize[1] - newHeight) / 2));
        newRectangle.getWidth() = newRectangle.getWidth().shift(newWidth - newRectangle.getWidth());
        newRectangle.getHeight() =  newRectangle.getHeight().shift(newHeight - newRectangle.getHeight());
        return newRectangle;
    }

    Rectangle Rectangle::withTopLeft(Point position)
    {
        Rectangle newRectangle = *this;
        newRectangle.setPosition(position);
        return newRectangle;
    }

    Rectangle Rectangle::withCenter(Point position)
    {
        Rectangle newRectangle = *this;
        newRectangle.mPosition = position;
        newRectangle.mPosition = newRectangle.mPosition.withOffset(Point(-newRectangle.getWidth() / 2.0, -newRectangle.getHeight() / 2.0));
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

    void Rectangle::setPosition(const Point& position) 
    { 
        mPosition = position; 
    }
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