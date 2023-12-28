#include "Color.h"

namespace pertyG
{
    // Constructors
    Color::Color(Property r, Property g, Property b, Property a)
        : red(r < 0.0f ? 0.0f : (r > 1.0f ? 1.0f : r)),
        green(g < 0.0f ? 0.0f : (g > 1.0f ? 1.0f : g)),
        blue(b < 0.0f ? 0.0f : (b > 1.0f ? 1.0f : b)),
        alpha(a < 0.0f ? 0.0f : (a > 1.0f ? 1.0f : a)) {}

    Color::Color(int r, int g, int b, int a)
        : red((r < 0 ? 0.0f : (r > 255 ? 1.0f : static_cast<double>(r) / 255.0f))),
        green((g < 0 ? 0.0f : (g > 255 ? 1.0f : static_cast<double>(g) / 255.0f))),
        blue((b < 0 ? 0.0f : (b > 255 ? 1.0f : static_cast<double>(b) / 255.0f))),
        alpha((a < 0 ? 0.0f : (a > 255 ? 1.0f : static_cast<double>(a) / 255.0f))) {}

    // Getter methods
    Property & Color::getRed()  { return red; }
    Property & Color::getGreen()  { return green; }
    Property & Color::getBlue()  { return blue; }
    Property & Color::getAlpha()  { return alpha; }

    // Setter methods
    void Color::setRed(double r) { red = (r < 0.0f ? 0.0f : (r > 1.0f ? 1.0f : r)); }
    void Color::setGreen(double g) { green = (g < 0.0f ? 0.0f : (g > 1.0f ? 1.0f : g)); }
    void Color::setBlue(double b) { blue = (b < 0.0f ? 0.0f : (b > 1.0f ? 1.0f : b)); }
    void Color::setAlpha(double a) { alpha = (a < 0.0f ? 0.0f : (a > 1.0f ? 1.0f : a)); }

    // Method to get the brightness of the color
    double Color::getBrightness()
    {
        return (0.299 * red + 0.587 * green + 0.114 * blue) * 100.0;
    }

    // Method to convert the color to a hexadecimal string
    std::string Color::toHex() 
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(red * 255.0f);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(green * 255.0f);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(blue * 255.0f);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(alpha * 255.0f);
        return ss.str();
    }

    // Method to blend two colors
    Color Color::blend( Color& other) 
    {
        float blendedRed = (red + other.red) / 2.0f;
        float blendedGreen = (green + other.green) / 2.0f;
        float blendedBlue = (blue + other.blue) / 2.0f;
        float blendedAlpha = (alpha + other.alpha) / 2.0f;
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
