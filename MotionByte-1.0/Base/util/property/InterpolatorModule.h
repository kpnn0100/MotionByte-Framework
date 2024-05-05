#pragma once
#include <math.h>
#include <iostream>
#include <vector>
#include "InterpolatorBase.h"
#include "debugging_tool/DEBUG.h"
namespace MotionByte
{
	class Property;
	class InterpolatorModule
	{
	protected:
		friend class InterpolatorFactory;
		friend class InterpolatorBase;
		//must have duration at least
		InterpolatorType mInterpolatorID;
		std::vector<double> mPropertyList;
		std::vector<double> mInterpolatorState;
	public:
		InterpolatorModule();
		InterpolatorModule(InterpolatorType id, int numberOfProperty);
		std::vector<double>& getPropertyList();
		std::vector<double>& getInterpolatorState();
		InterpolatorType getInterpolatorType();
		void updateStateFor(Property& property);
		bool isSet(Property& property);
		double getValueAtTime(Property & property);
		double getVelocityAtTime(Property& property);
	};
}
