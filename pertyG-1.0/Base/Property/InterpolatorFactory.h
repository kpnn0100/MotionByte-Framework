#pragma once
#include "Interpolator/Step.h"
#include <memory>
namespace pertyG
{
	class InterpolatorFactory
	{
	public:
		static std::shared_ptr<Interpolator> createStep()
		{
			return std::make_shared<Step>();
		}
	};
}
