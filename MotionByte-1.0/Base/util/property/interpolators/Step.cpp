#include "../PropertyHeader.h"
namespace MotionByte
{
	InterpolatorModule Step::create()
	{
		InterpolatorModule step(InterpolatorType::STEP, 0);
		return step;
	}
	bool Step::isSet(Property& property)
	{
		return true;
	}
	double Step::getValueAtTime(Property& property)
	{
		return property.getTargetValue();
	}
	double Step::getVelocityAtTime(Property& property)
	{
		return 0;
	}
	void Step::updateStateFor(Property& property)
	{
	}
}
