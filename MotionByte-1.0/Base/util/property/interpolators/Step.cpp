#include "../PropertyHeader.h"
namespace MotionByte
{
	Step::Step() : Interpolator(0)
	{

	}
	bool Step::isSet(Property& property)
	{
		return true;
	}
	double Step::getValueAtTime(Property& property)
	{
		return property.getTargetValue();
	}
}
