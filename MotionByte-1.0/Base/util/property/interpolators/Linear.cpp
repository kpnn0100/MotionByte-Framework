#include "../PropertyHeader.h"
namespace MotionByte
{
	Linear::Linear(double duration) : Interpolator(1)
	{
		mPropertyList[ParameterList::Duration] = duration;
	}
	bool Linear::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		if (time > mPropertyList[ParameterList::Duration])
		{
			return true;
		}
		return false;
	}
	double Linear::getValueAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		if (time > mPropertyList[ParameterList::Duration])
		{
			return targetValue;
		}
		return initValue + time/ mPropertyList[ParameterList::Duration] * (targetValue- initValue);
	}
	double Linear::getVelocityAtTime(Property& property)
	{
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		return (targetValue- initValue)/ mPropertyList[ParameterList::Duration];
	}
}
