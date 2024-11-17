#pragma once
#include <cmath>

namespace MotionByte
{
    // Represents a 2D point

    // Convert degrees to radians
    class Point;
    double toRadians(double degrees);

    // Calculate the distance between two points
    double distance(Point& p1, Point& p2);


    // Linear interpolation between two values
    double lerp(double a, double b, double t);

    // Clamp a value to a specified range
    double clamp(double value, double min, double max);

    // Other mathematical functions can be added based on your requirements
    double getNegativeSign(double value);
}
