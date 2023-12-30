#include "../PropertyHeader.h"
namespace pertyG
{
	EaseInOut::EaseInOut(double duration) : Interpolator(ParameterCount)
	{
		mPropertyList[ParameterList::Duration] = duration;
	}

	bool EaseInOut::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		return time > mPropertyList[ParameterList::Duration];
	}

	double EaseInOut::getValueAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		if (time > mPropertyList[ParameterList::Duration])
		{
			return targetValue;
		}

		double t = time / mPropertyList[ParameterList::Duration];
		t /= 0.5;
		if (t < 1.0) {
			return initValue + (targetValue - initValue) * 0.5 * t * t * t;
		}
		t -= 2.0;
		return initValue + (targetValue - initValue) * 0.5 * (t * t * t + 2.0);
	}
}