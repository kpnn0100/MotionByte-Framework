#pragma once
#include "../Interpolator.h"
namespace pertyG
{
	class Step : public Interpolator
	{
	public:
		Step();
		virtual bool isSet(double time) override;
		virtual double getValueAtTime(double initValue, double targetValue, double time) override;
	};
}
