#include "../PropertyHeader.h"
namespace pertyG
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
