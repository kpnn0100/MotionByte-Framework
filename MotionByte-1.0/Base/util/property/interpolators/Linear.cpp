#include "../PropertyHeader.h"
namespace MotionByte
{
	InterpolatorModule Linear::create(double duration)
	{
		InterpolatorModule linear(InterpolatorType::LINEAR, ParameterList::ParameterCount);
		linear.getPropertyList()[ParameterList::Duration] = duration;
		return linear;
	}
	bool Linear::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		if (time > property.getInterpolator().getPropertyList()[ParameterList::Duration])
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
		if (time > property.getInterpolator().getPropertyList()[ParameterList::Duration])
		{
			return targetValue;
		}
		return initValue + time/ property.getInterpolator().getPropertyList()[ParameterList::Duration] * (targetValue- initValue);
	}
	double Linear::getVelocityAtTime(Property& property)
	{
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		return (targetValue- initValue)/ property.getInterpolator().getPropertyList()[ParameterList::Duration];
	}
	void Linear::updateStateFor(Property& property)
	{
	}
}
