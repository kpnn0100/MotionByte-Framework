#pragma once
#include <cmath>
#include "../Interpolator.h"

namespace MotionByte
{
	class Smooth : public Interpolator
	{
	private:
		inline bool isHavingSustainPhase(double initV2, double expectedV2, double initValue, double finalValue, double acc);
	public:
		enum ParameterList
		{
			Accelerator,
			ExpectedVelocity,
			ParameterCount
		};
		enum StateList
		{
			IsHavingSustainPhase,
			InitVelocity,
			TargetVelocity,
			InitValue,
			TargetValue,
			Acceleration,
			AcceleratingDuration,
			AcceleratingDistance,
			DeceleratingDuration,
			DeceleratingDistance,
			MaintainingDuration,
			MaintainingDistance,
			OffsetDuration,
			OffsetDistance,
			SizeOfState
			
		};
		Smooth(double accelerator, double expectedVelocity);
		virtual void updateStateFor(Property& property) override;
		virtual bool isSet(Property& property) override;
		virtual double getVelocityAtTime(Property& property) override;
		virtual double getValueAtTime(Property& property) override;
	};
}
