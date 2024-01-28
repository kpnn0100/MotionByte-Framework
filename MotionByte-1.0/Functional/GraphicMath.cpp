#include "GraphicMath.h"
#include "../Base/Segment/Point.h"
namespace MotionByte
{
    double toRadians(double degrees) {
        return degrees * static_cast<double>(3.141592653589) / 180.0f;
    }

    // Calculate the distance between two points
    double distance(Point& p1, Point& p2) {
        double dx = p2.getX() - p1.getX();
        double dy = p2.getY() - p1.getY();
        return std::sqrt(dx * dx + dy * dy);
    }

    // Linear interpolation between two values
    double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    // Clamp a value to a specified range
    double clamp(double value, double min, double max) {
        if (value < min)
        {
            return min;
        }
        if (value > max)
        {
            return max;
        }
        return value;
    }
}