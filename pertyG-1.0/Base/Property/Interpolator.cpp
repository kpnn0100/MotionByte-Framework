#pragma once
#include "PropertyHeader.h"
namespace pertyG
{
	Interpolator::Interpolator(int numberOfProperty)
	{
		mPropertyList.resize(numberOfProperty);
	}
	double Interpolator::getVelocityAtTime(Property& property)
	{
		return 0.0;
	}
}
