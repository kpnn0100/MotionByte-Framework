#include "EaseInOut.h"

pertyG::EaseInOut::EaseInOut(double duration) : Interpolator(ParameterCount)
{
	mPropertyList[ParameterList::Duration] = duration;
}

bool pertyG::EaseInOut::isSet(double time)
{
	return time > mPropertyList[ParameterList::Duration];
}

double pertyG::EaseInOut::getValueAtTime(double initValue, double targetValue, double time)
{
	if (time > mPropertyList[ParameterList::Duration])
	{
		return targetValue;
	}

	double t = time / mPropertyList[ParameterList::Duration];
	t /= 0.5;
	if (t < 1.0) {
		return initValue + (targetValue- initValue)*0.5 * t * t * t;
	}
	t -= 2.0;
	return initValue + (targetValue - initValue) * 0.5 * (t * t * t + 2.0);
}
