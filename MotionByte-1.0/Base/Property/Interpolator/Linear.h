#pragma once
#include "../Interpolator.h"
namespace MotionByte
{
	class Linear : public Interpolator
	{
		enum ParameterList
		{
			Duration,
			ParameterCount
		};
	public:
		Linear(double duration);
		virtual bool isSet(Property& property) override;
		virtual double getValueAtTime(Property& property) override;
		virtual double getVelocityAtTime(Property& property) override;
	};
}
