#pragma once
#include <cmath>
#include "../Base/Segment/Point.h"
namespace MotionByte
{
    // Represents a 2D point

    // Convert degrees to radians
    inline float toRadians(float degrees) {
        return degrees * static_cast<float>(3.141592653589) / 180.0f;
    }

    // Calculate the distance between two points
    inline float distance(Point& p1, Point& p2) {
        float dx = p2.getX() - p1.getX();
        float dy = p2.getY() - p1.getY();
        return std::sqrt(dx * dx + dy * dy);
    }


    // Linear interpolation between two values
    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    // Clamp a value to a specified range
    inline float clamp(float value, float min, float max) {
        return std::max(min, std::min(value, max));
    }

    // Other mathematical functions can be added based on your requirements
}
