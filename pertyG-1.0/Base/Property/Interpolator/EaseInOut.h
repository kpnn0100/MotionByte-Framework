#pragma once
#pragma once
#include <cmath>
#include "../Interpolator.h"

namespace pertyG
{
	class EaseInOut : public Interpolator
	{
	public:
		enum ParameterList
		{
			Duration,
			ParameterCount
		};
		EaseInOut(double duration);

		bool isSet(double time) override;

		double getValueAtTime(double initValue, double targetValue, double time) override;
	};
}
