#pragma once
#include "Step.h"
namespace pertyG
{
	Step::Step() : Interpolator(0)
	{

	}
	bool Step::isSet(double time)
	{
		return true;
	}
	double Step::getValueAtTime(double initValue, double targetValue, double time)
	{
		return targetValue;
	}
}
