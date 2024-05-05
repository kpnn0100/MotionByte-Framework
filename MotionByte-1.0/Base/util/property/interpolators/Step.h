#pragma once
#include "../InterpolatorModule.h"
namespace MotionByte
{
	class Step : public InterpolatorBase
	{
	
	public:
		static InterpolatorModule create();
		static bool isSet(Property& property);
		static double getValueAtTime(Property& property);
		static double getVelocityAtTime(Property& property);
		static void updateStateFor(Property& property);
	};
}
