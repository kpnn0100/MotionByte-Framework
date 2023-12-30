#pragma once
#include <math.h>
#include <iostream>
#include <vector>
namespace pertyG
{
	class Property;
	class Interpolator
	{
	protected:
		//must have duration at least
		std::vector<double> mPropertyList;
	public:
		Interpolator() = delete;
		Interpolator(int numberOfProperty);
		virtual bool isSet(Property& property) = 0;
		virtual double getValueAtTime(Property & property) = 0;
		virtual double getVelocityAtTime(Property& property);
	};
}
