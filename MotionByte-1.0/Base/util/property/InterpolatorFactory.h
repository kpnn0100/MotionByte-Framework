#pragma once
#include "interpolators/Step.h"
#include "interpolators/Linear.h"
#include "interpolators/EaseInOut.h"
#include "interpolators/Smooth.h"
#include <memory>
namespace MotionByte
{
	class InterpolatorFactory
	{
	public:
		static InterpolatorModule createStep();
		static InterpolatorModule createLinear(double duration);
		static InterpolatorModule createEaseInOut(double duration);
		static InterpolatorModule createSmooth(double acceleration, double expectedVelocity);

		static void updateStateFor(Property& property);
		static bool isSet(Property& property);
		static double getVelocityAtTime(Property& property);
		static double getValueAtTime(Property& property);
	};
}
