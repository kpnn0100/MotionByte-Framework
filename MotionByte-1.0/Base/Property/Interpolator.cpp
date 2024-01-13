#pragma once
#include "PropertyHeader.h"
namespace MotionByte
{
	Interpolator::Interpolator(int numberOfProperty)
	{
		mPropertyList.resize(numberOfProperty);
	}
	void Interpolator::updateStateFor(Property& property)
	{
	}
	double Interpolator::getVelocityAtTime(Property& property)
	{
		return 0.0;
	}
}
