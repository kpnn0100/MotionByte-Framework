#pragma once
#include "../Property.h"
namespace pertyG
{
class Point {
private:
    Property mPosition[2];
public:
    enum { X, Y };
    Point() : mPosition{0.0, 0.0} {}

    Point(double x, double y) : mPosition{x, y} {}

    Property getX() const { return mPosition[X]; }
    Property getY() const { return mPosition[Y]; }
    Property getPosition(int dimension)
    {
        mPosition[dimension];
    }
    void setPosition(int dimension, double value) {
        if (dimension >= 0 && dimension < 2) {
            mPosition[dimension].setValue( value);
        }
    }
};
}