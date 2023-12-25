#pragma once
#include "Linear.h"
namespace pertyG
{
	Linear::Linear(double duration) : Interpolator(1)
	{
		mPropertyList[ParameterList::Duration] = duration;
	}
	bool Linear::isSet(double time)
	{
		if (time > mPropertyList[ParameterList::Duration])
		{
			return true;
		}
		return false;
	}
	double Linear::getValueAtTime(double initValue, double targetValue, double time)
	{
		if (time > mPropertyList[ParameterList::Duration])
		{
			return targetValue;
		}
		return initValue + time/ mPropertyList[ParameterList::Duration] * (targetValue- initValue);
	}
}
