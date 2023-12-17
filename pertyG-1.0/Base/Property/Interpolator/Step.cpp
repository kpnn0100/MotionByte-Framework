#pragma once
#include "Step.h"
namespace pertyG
{
	Step::Step() : Interpolator(0)
	{

	}
	bool Step::isSet(double time)
	{
		if (time > 0.0)
		{
			return true;
		}
		return false;
	}
	double Step::getValueAtTime(double initValue, double targetValue, double time)
	{
		if (time > 0.0)
		{
			return targetValue;
		}
		return initValue;
	}
}
