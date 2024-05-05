#pragma once
#include "PropertyHeader.h"
namespace MotionByte
{
	InterpolatorModule InterpolatorFactory::createStep()
	{
		return Step::create();
	}
	InterpolatorModule InterpolatorFactory::createLinear(double duration)
	{
		return Linear::create(duration);
	}
	InterpolatorModule InterpolatorFactory::createEaseInOut(double duration)
	{
		return EaseInOut::create(duration);
	}
	InterpolatorModule InterpolatorFactory::createSmooth(double acceleration, double expectedVelocity)
	{
		return  Smooth::create(acceleration,expectedVelocity);
	}
	void InterpolatorFactory::updateStateFor(Property& property)
	{
		auto type = property.getInterpolatorType();
		switch (type)
		{
			case InterpolatorType::STEP:
				Step::updateStateFor(property);
				break;
			case InterpolatorType::LINEAR:
				Linear::updateStateFor(property);
				break;
			case InterpolatorType::EASE_IN_OUT:
				EaseInOut::updateStateFor(property);
				break;
			case InterpolatorType::SMOOTH:
				Smooth::updateStateFor(property);
				break;
			default:
				break;
		}
	}
	bool InterpolatorFactory::isSet(Property& property)
	{
		auto type = property.getInterpolatorType();
		switch (type)
		{
			case InterpolatorType::STEP:
				return Step::isSet(property);
			case InterpolatorType::LINEAR:
				return Linear::isSet(property);
			case InterpolatorType::EASE_IN_OUT:
				return EaseInOut::isSet(property);
			case InterpolatorType::SMOOTH:
				return Smooth::isSet(property);
			default:
				return true;
		}
	}
	double InterpolatorFactory::getVelocityAtTime(Property& property)
	{
		auto type = property.getInterpolatorType();
		switch (type)
		{
			case InterpolatorType::STEP:
				return Step::getVelocityAtTime(property);
			case InterpolatorType::LINEAR:
				return Linear::getVelocityAtTime(property);
			case InterpolatorType::EASE_IN_OUT:
				return EaseInOut::getVelocityAtTime(property);
			case InterpolatorType::SMOOTH:
				return Smooth::getVelocityAtTime(property);
			default:
				return 0;
		}
	}
	double InterpolatorFactory::getValueAtTime(Property& property)
	{
		auto type = property.getInterpolatorType();
		switch (type)
		{
			case InterpolatorType::STEP:
				return Step::getValueAtTime(property);
			case InterpolatorType::LINEAR:
				return Linear::getValueAtTime(property);
			case InterpolatorType::EASE_IN_OUT:
				return EaseInOut::getValueAtTime(property);
			case InterpolatorType::SMOOTH:
				return Smooth::getValueAtTime(property);
			default:
				return 0;
		}
	}
}
