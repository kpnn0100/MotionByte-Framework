#pragma once
#include <cmath>
#include "../InterpolatorModule.h"

namespace MotionByte
{
	class Smooth : public InterpolatorBase
	{
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
