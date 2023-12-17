#pragma once
#include <math.h>
#include <vector>
namespace pertyG
{
	class Interpolator
	{
	protected:
		//must have duration at least
		std::vector<double> mPropertyList;
	public:
		Interpolator() = delete;
		Interpolator(int numberOfProperty);
		virtual bool isSet(double time) = 0;
		virtual double getValueAtTime(double initValue, double targetValue, double time) = 0;
	};
}
