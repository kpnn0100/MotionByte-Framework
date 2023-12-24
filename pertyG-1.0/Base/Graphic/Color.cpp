#include "Color.h"
namespace pertyG
{
		// Calculate brightness as a percentage
	double Color::getBrightness() const
	{
		return (0.299 * red + 0.587 * green + 0.114 * blue) / 255.0 * 100.0;
	}

	// Convert the color to a hexadecimal string
	std::string Color::toHex() const
	{
		std::stringstream ss;
		ss << std::hex << std::setw(2) << std::setfill('0') << red;
		ss << std::hex << std::setw(2) << std::setfill('0') << green;
		ss << std::hex << std::setw(2) << std::setfill('0') << blue;
		ss << std::hex << std::setw(2) << std::setfill('0') << alpha;
		return ss.str();
	}

	// Blend two colors by taking the average of their RGB values
	Color Color::blend(const Color& other) const
	{
		int blendedRed = (red + other.red) / 2;
		int blendedGreen = (green + other.green) / 2;
		int blendedBlue = (blue + other.blue) / 2;
		int blendedAlpha = (alpha + other.alpha) / 2;
		return Color(blendedRed, blendedGreen, blendedBlue, blendedAlpha);
	}
	// Get a brighter version of the color
	Color Color::brighter(double factor) const
	{
		int newRed = static_cast<int>(red + 255 * factor);
		int newGreen = static_cast<int>(green + 255 * factor);
		int newBlue = static_cast<int>(blue + 255 * factor);

		if (newRed > 255) newRed = 255;
		if (newGreen > 255) newGreen = 255;
		if (newBlue > 255) newBlue = 255;

		return Color(newRed, newGreen, newBlue, alpha);
	}

	// Get a darker version of the color
	Color Color::darker(double factor) const
	{
		int newRed = static_cast<int>(red * (1.0 - factor));
		int newGreen = static_cast<int>(green * (1.0 - factor));
		int newBlue = static_cast<int>(blue * (1.0 - factor));

		if (newRed < 0) newRed = 0;
		if (newGreen < 0) newGreen = 0;
		if (newBlue < 0) newBlue = 0;

		return Color(newRed, newGreen, newBlue, alpha);
	}
}