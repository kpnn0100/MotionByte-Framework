#pragma once
#include "../InterpolatorModule.h"
namespace MotionByte
{
	class Linear : public InterpolatorBase
	{
		enum ParameterList
		{
			Duration,
			ParameterCount
		};
	public:
		static InterpolatorModule create(double duration);
		static bool isSet(Property& property);
		static double getValueAtTime(Property& property);
		static double getVelocityAtTime(Property& property);
		static void updateStateFor(Property& property);
	};
}
