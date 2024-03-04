#pragma once
#include "Interpolator/Step.h"
#include "Interpolator/Linear.h"
#include "Interpolator/EaseInOut.h"
#include "Interpolator/Smooth.h"
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
