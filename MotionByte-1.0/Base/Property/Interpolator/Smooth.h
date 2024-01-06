#pragma once
#include <cmath>
#include "../Interpolator.h"

namespace MotionByte
{
	class Smooth : public Interpolator
	{
	public:
		enum ParameterList
		{
			Accelerator,
			ExpectedVelocity,
			ParameterCount
		};
		Smooth(double accelerator, double expectedVelocity);

		bool isSet(Property& property) override;
		virtual double getVelocityAtTime(Property& property) override;
		virtual double getValueAtTime(Property& property) override;
	};
}
