#pragma once
#include "../Property/Property.h"
namespace pertyG
{
    class Point {
    private:
        Property mPosition[2];
    public:
        enum { X, Y };
        Point();
        Point(double x, double y);
        Point(const Point& other);
        Point operator=(const Point& other);
        Property &getX() ;
        Property &getY() ;
        Property &getPosition(int dimension);
        void setPosition(int dimension, double value);
        void move(Point vector);
        Point withOffset(Point offset);
    };
}