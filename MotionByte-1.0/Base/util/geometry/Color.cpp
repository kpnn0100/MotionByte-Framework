#include "util/geometry/Color.h"

namespace MotionByte
{
    Color::Color():Color(0.0,0.0,0.0,0.0)
    {
    }
    // Constructors
    Color::Color(Property r, Property g, Property b, Property a) :
        Color((double)r, (double)g, (double)b, (double)a)
    {}

    Color::Color(double r, double g, double b, double a)
        : red(r < 0.0 ? Property(0.0) : (r > 1.0 ? Property(1.0) : Property(r))),
        green(g < 0.0 ? Property(0.0) : (g > 1.0 ? Property(1.0) : Property(g))),
        blue(b < 0.0 ? Property(0.0) : (b > 1.0 ? Property(1.0) : Property(b))),
        alpha(a < 0.0 ? Property(0.0) : (a > 1.0 ? Property(1.0) : Property(a)))
    {
    }

    Color::Color(int r, int g, int b, int a)
        : red((r < 0 ? 0.0 : (r > 255 ? 1.0 : static_cast<double>(r) / 255.0))),
        green((g < 0 ? 0.0 : (g > 255 ? 1.0 : static_cast<double>(g) / 255.0))),
        blue((b < 0 ? 0.0 : (b > 255 ? 1.0 : static_cast<double>(b) / 255.0))),
        alpha((a < 0 ? 0.0 : (a > 255 ? 1.0 : static_cast<double>(a) / 255.0))) {}

    void Color::setColor(Color color)
    {
        red = color.red;
        green = color.green;
        blue = color.blue;
        alpha = color.alpha;
    }

    void Color::setInterpolator(std::shared_ptr<Interpolator> interpolator)
    {
        red.setInterpolator(interpolator);
        green.setInterpolator(interpolator);
        blue.setInterpolator(interpolator);
        alpha.setInterpolator(interpolator);
    }

    void Color::initColor(Color color)
    {
        red.initValue(color.red);
        green.initValue(color.green);
        blue.initValue(color.blue);
        alpha.initValue(color.alpha);
    }

    Color Color::getColor()
    {
        return Color(red,green,blue,alpha);
    }

    Color Color::getTargetColor()
    {
        return Color(red.getTargetValue(),
            green.getTargetValue(),
            blue.getTargetValue(),
            alpha.getTargetValue());
    }

    // Getter methods
    Property & Color::getRed()  { return red; }
    Property & Color::getGreen()  { return green; }
    Property & Color::getBlue()  { return blue; }
    Property & Color::getAlpha()  { return alpha; }

    // Setter methods
    void Color::setRed(double r) { red = (r < 0.0 ? 0.0 : (r > 1.0 ? 1.0 : r)); }
    void Color::setGreen(double g) { green = (g < 0.0 ? 0.0 : (g > 1.0 ? 1.0 : g)); }
    void Color::setBlue(double b) { blue = (b < 0.0 ? 0.0 : (b > 1.0 ? 1.0 : b)); }
    void Color::setAlpha(double a) { alpha = (a < 0.0 ? 0.0 : (a > 1.0 ? 1.0 : a)); }

    // Method to get the brightness of the color
    double Color::getBrightness()
    {
        return (0.299 * red + 0.587 * green + 0.114 * blue) * 100.0;
    }

    // Method to convert the color to a hexadecimal string
    std::string Color::toHex() 
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(red * 255.0);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(green * 255.0);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(blue * 255.0);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(alpha * 255.0);
        return ss.str();
    }

    // Method to blend two colors
    Color Color::blend( Color& other) 
    {
        float blendedRed = (red + other.red) / 2.0;
        float blendedGreen = (green + other.green) / 2.0;
        float blendedBlue = (blue + other.blue) / 2.0;
        float blendedAlpha = (alpha + other.alpha) / 2.0;
        return Color(blendedRed, blendedGreen, blendedBlue, blendedAlpha);
    }

    // Method to get a brighter version of the color
    Color Color::brighter(double factor) 
    {
        Color newColor = *this;
        newColor.red.shift(factor);
        if (newColor.red.getTargetValue() > 1.0)
        {
            newColor.red.shift(-(newColor.red.getTargetValue() - 1.0));
        }
        newColor.green.shift(static_cast<double>(factor));
        if (newColor.green.getTargetValue() > 1.0)
        {
            newColor.green.shift(-(newColor.green.getTargetValue() - 1.0));
        }
        newColor.blue.shift(static_cast<double>(factor));
        if (newColor.blue.getTargetValue() > 1.0)
        {
            newColor.blue.shift(-(newColor.blue.getTargetValue() - 1.0));
        }
        return newColor;
    }

    // Method to get a darker version of the color
    Color Color::darker(double factor) 
    {
        Color newColor = *this;
        newColor.red.shift(-factor);
        if (newColor.red.getTargetValue() < 0.0)
        {
            newColor.red.shift(-newColor.red.getTargetValue());
        }
        newColor.green.shift(-factor);
        if (newColor.green.getTargetValue() < 0.0)
        {
            newColor.green.shift(-newColor.green.getTargetValue());
        }
        newColor.blue.shift(-factor);
        if (newColor.blue.getTargetValue() < 0.0)
        {
            newColor.blue.shift(-newColor.blue.getTargetValue());
        }
        return newColor;
    }
}
