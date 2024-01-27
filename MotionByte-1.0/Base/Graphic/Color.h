#pragma once
#include <sstream>
#include <iomanip>
#include "../Property/Property.h"
namespace MotionByte
{
    class Color
    {
    private:
        Property red;
        Property green;
        Property blue;
        Property alpha;

    public:
        // Constructors
        Color(Property r = 0.0f, Property g = 0.0f, Property b = 0.0f, Property a = 1.0f);
        Color(double r = 0.0f, double g = 0.0f, double b = 0.0f, double a = 1.0f);
        Color(int r, int g, int b, int a = 255);

        void setColor(Color color);
        void setInterpolator(std::shared_ptr<Interpolator> interpolator);
        void initColor(Color color);
        Color getColor();
        Color getTargetColor();
        // Getter methods
        Property & getRed() ;
        Property & getGreen() ;
        Property & getBlue() ;
        Property & getAlpha() ;

        // Setter methods
        void setRed(double r);
        void setGreen(double g);
        void setBlue(double b);
        void setAlpha(double a);

        // Method to get the brightness of the color
        double getBrightness() ;

        // Method to convert the color to a hexadecimal string
        std::string toHex() ;

        // Method to blend two colors
        Color blend( Color& other) ;

        // Method to get a brighter version of the color
        Color brighter(double factor = 0.7) ;

        // Method to get a darker version of the color
        Color darker(double factor = 0.7) ;


    };
}
