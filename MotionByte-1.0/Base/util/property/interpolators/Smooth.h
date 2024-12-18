#pragma once
#include <cmath>
#include "../InterpolatorModule.h"

namespace MotionByte
{
	class Smooth : public InterpolatorBase
	{
	private:
		static inline bool isHavingSustainPhase( double initV2, double expectedV2, double initValue, double finalValue, double acc);
		static inline bool isHavingAcceleratingPhase(double initV, double expectedV, double initValue, double finalValue, double acc);
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
			IsHavingAcceleratingPhase,
			InitVelocity,
			MiddleVelocity,
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
			SizeOfState
			
		};
		static InterpolatorModule create(double accelerator, double expectedVelocity);
		static void updateStateFor(Property& property);
		static bool isSet(Property& property);
		static double getVelocityAtTime(Property& property);
		static double getValueAtTime(Property& property);
	};
}
