#pragma once
#include "../Interpolator.h"
namespace pertyG
{
	class Step : public Interpolator
	{
	public:
		Step();
		virtual bool isSet(Property& property) override;
		virtual double getValueAtTime(Property& property) override;
	};
}
