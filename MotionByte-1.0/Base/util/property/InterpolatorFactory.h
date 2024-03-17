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
		static std::shared_ptr<Interpolator> createStep();
		static std::shared_ptr<Interpolator> createLinear(double duration);
		static std::shared_ptr<Interpolator> createEaseInOut(double duration);
		static std::shared_ptr<Interpolator> createSmooth(double acceleration, double expectedVelocity);
	};
}
