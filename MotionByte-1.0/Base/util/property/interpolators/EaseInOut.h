#pragma once
#include <cmath>
#include "../InterpolatorModule.h"

namespace MotionByte
{
	class EaseInOut : public InterpolatorBase
	{
	public:
		enum ParameterList
		{
			Duration,
			ParameterCount
		};
		static InterpolatorModule create(double duration);

		static bool isSet(Property& property);
		static double getVelocityAtTime(Property& property);
		static double getValueAtTime(Property& property);
		static void updateStateFor(Property& property);
	};
}
