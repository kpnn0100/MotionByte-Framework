#pragma once
#include "../Interpolator.h"
namespace pertyG
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
		virtual bool isSet(double time) override;
		virtual double getValueAtTime(double initValue, double targetValue, double time) override;
	};
}
