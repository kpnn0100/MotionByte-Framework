#include "../PropertyHeader.h"
namespace MotionByte
{
	InterpolatorModule EaseInOut::create(double duration)
	{
		InterpolatorModule easeInOut(InterpolatorType::EASE_IN_OUT, ParameterList::ParameterCount);
		easeInOut.getPropertyList()[ParameterList::Duration] = duration;
		return easeInOut;
	}

	bool EaseInOut::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		return time > property.getInterpolator().getPropertyList()[ParameterList::Duration];
	}
	double EaseInOut::getVelocityAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		double duration = property.getInterpolator().getPropertyList()[ParameterList::Duration];

		if (time >= duration)
		{
			// Velocity is zero when the interpolation is complete
			return 0.0;
		}

		double t = time / duration;
		double dt = 1.0 / duration;

		// Calculate velocity using the derivative of the interpolation function
		if (t < 0.5) {
			return 1.5 * (targetValue - initValue) * t * t / duration;
		}
		else {
			t -= 0.5;
			return 1.5 * (targetValue - initValue) * (3.0 * t * t + 1.0) / duration;
		}
	}
	double EaseInOut::getValueAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		if (time > property.getInterpolator().getPropertyList()[ParameterList::Duration])
		{
			return targetValue;
		}

		double t = time / property.getInterpolator().getPropertyList()[ParameterList::Duration];
		t /= 0.5;
		if (t < 1.0) {
			return initValue + (targetValue - initValue) * 0.5 * t * t * t;
		}
		t -= 2.0;
		return initValue + (targetValue - initValue) * 0.5 * (t * t * t + 2.0);
	}
	void EaseInOut::updateStateFor(Property& property)
	{
	}
}