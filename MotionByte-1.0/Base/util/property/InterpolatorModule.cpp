#pragma once
#include "PropertyHeader.h"
namespace MotionByte
{
	InterpolatorModule::InterpolatorModule(): InterpolatorModule(InterpolatorType::STEP,0)
	{
	}
	InterpolatorModule::InterpolatorModule(InterpolatorType id, int numberOfProperty): mInterpolatorID(id)
	{
		mPropertyList.resize(numberOfProperty);
	}
	std::vector<double>& InterpolatorModule::getPropertyList()
	{
		return mPropertyList;
	}
	std::vector<double>& InterpolatorModule::getInterpolatorState()
	{
		return mInterpolatorState;
	}
	InterpolatorType InterpolatorModule::getInterpolatorType()
	{
		return mInterpolatorID;
	}
	void InterpolatorModule::updateStateFor(Property& property)
	{
		InterpolatorFactory::updateStateFor(property);
	}
	double InterpolatorModule::getVelocityAtTime(Property& property)
	{
		return InterpolatorFactory::getVelocityAtTime(property);
	}
	double InterpolatorModule::getValueAtTime(Property& property)
	{
		return InterpolatorFactory::getValueAtTime(property);
	}
	bool InterpolatorModule::isSet(Property& property)
	{
		return InterpolatorFactory::isSet(property);
	}
}
