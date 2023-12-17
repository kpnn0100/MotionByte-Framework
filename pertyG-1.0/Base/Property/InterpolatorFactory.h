#pragma once
#include "Interpolator/Step.h"
#include "Interpolator/Linear.h"
#include <memory>
namespace pertyG
{
	class InterpolatorFactory
	{
	public:
		static std::shared_ptr<Interpolator> createStep();
		static std::shared_ptr<Interpolator> createLinear(double duration);
	};
}
