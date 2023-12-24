#pragma once
#include <algorithm> // for std::clamp
#include <sstream>
#include <iomanip>
namespace pertyG
{
	class Color
	{
	private:
		int red;
		int green;
		int blue;
		int alpha;

	public:
		// Constructors
		Color(int r = 0, int g = 0, int b = 0, int a = 255)
			: red(r), green(g), blue(b), alpha(a) {}

		// Getter methods
		int getRed() const { return red; }
		int getGreen() const { return green; }
		int getBlue() const { return blue; }
		int getAlpha() const { return alpha; }

		// Setter methods
		void setRed(int r) { red = r; }
		void setGreen(int g) { green = g; }
		void setBlue(int b) { blue = b; }
		void setAlpha(int a) { alpha = a; }

		// Method to get the brightness of the color
		double getBrightness() const;

		// Method to convert the color to a hexadecimal string
		std::string toHex() const;

		// Method to blend two colors
		Color blend(const Color& other) const;
		// Method to get a brighter version of the color
		Color brighter(double factor = 0.7) const;

		// Method to get a darker version of the color
		Color darker(double factor = 0.7) const;
	};


}
