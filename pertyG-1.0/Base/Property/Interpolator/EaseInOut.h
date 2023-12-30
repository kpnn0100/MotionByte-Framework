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

		bool isSet(Property& property) override;

		virtual double getValueAtTime(Property& property) override;
	};
}
