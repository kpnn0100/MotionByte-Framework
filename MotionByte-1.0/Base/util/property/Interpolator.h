#pragma once
#include <math.h>
#include <iostream>
#include <vector>
#include "../../DEBUG.h"
namespace MotionByte
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
		virtual void updateStateFor(Property& property);
		virtual bool isSet(Property& property) = 0;
		virtual double getValueAtTime(Property & property) = 0;
		virtual double getVelocityAtTime(Property& property);
	};
}
