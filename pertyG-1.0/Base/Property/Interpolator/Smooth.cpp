#include "../PropertyHeader.h"
#include <math.h>
namespace pertyG
{
	Smooth::Smooth(double accelerator, double expectedVelocity) : Interpolator(ParameterCount)
	{
		mPropertyList[ParameterList::Accelerator] = accelerator;
		mPropertyList[ParameterList::ExpectedVelocity] = expectedVelocity;
	}
	bool Smooth::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		double initVelocity = property.getLastVelocity();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		double acc = mPropertyList[ParameterList::Accelerator];
		double expectedV = mPropertyList[ParameterList::ExpectedVelocity];
		//vf^2 - vi^2 = 2 a s;
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		double expectedDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
			/ (2 * acc);
		double expectedFinalDistance = (0 - (expectedV * expectedV))
			/ (2 * -acc);
		//normal case
		if (std::abs(expectedDistance + expectedFinalDistance) < std::abs(targetValue - initValue))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
				/ (2 * acc);
			double finalDistance = (0 - (expectedV * expectedV))
				/ (2 * -acc);
			double EndPhaseDuration = (0 - expectedV) / -acc;
			double maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance;
			double MaintainPhaseDuration = maintainPhaseDistance / expectedV;
			if (time < AcceleratingPhaseDuration + MaintainPhaseDuration + EndPhaseDuration)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			//vf^2 - vi^2 = (target -init) * 2a - vf^2
			//vf = peak velocity
			double vf = (targetValue - initValue) * 2.0 * acc + initVelocity * initVelocity;
			vf = vf / 2.0;
			vf = std::sqrt(vf);
			vf = vf * expectedV / std::abs(expectedV);
			double AcceleratingPhaseDuration = (vf - initVelocity) / acc;
			double breakValue = initValue + initVelocity * AcceleratingPhaseDuration
				+ 1.0 / 2.0 * acc * AcceleratingPhaseDuration * AcceleratingPhaseDuration;
			double EndPhaseDuration = (0 - vf) / -acc;
			if (time < AcceleratingPhaseDuration + EndPhaseDuration)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	double Smooth::getVelocityAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initVelocity = property.getLastVelocity();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		double acc = mPropertyList[ParameterList::Accelerator];
		double expectedV = mPropertyList[ParameterList::ExpectedVelocity];
		if (targetValue == initValue)
		{
			return 0.0;
		}
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		//vf^2 - vi^2 = 2 a s;
		double expectedDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
			/ (2.0 * acc);
		double expectedFinalDistance = (0 - (expectedV * expectedV))
			/ (2.0 * -acc);
		//normal case
		if (std::abs(expectedDistance + expectedFinalDistance) < std::abs(targetValue - initValue))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
				/ (2 * acc);
			double finalDistance = (0 - (expectedV * expectedV))
				/ (2.0 * -acc);
			double EndPhaseDuration = (0 - expectedV) / -acc;
			double maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance;
			double MaintainPhaseDuration = maintainPhaseDistance / expectedV;

			if (time < AcceleratingPhaseDuration)
			{
				return initVelocity + acc * time;
			}
			else if (time < AcceleratingPhaseDuration + MaintainPhaseDuration)
			{
				return expectedV;
			}
			else if (time < AcceleratingPhaseDuration + MaintainPhaseDuration + EndPhaseDuration)
			{
				return expectedV + 2.0 * -acc * time;
			}
			else
			{
				return 0.0;
			}
		}
		else
		{
			//vf^2 - vi^2 = (target -init) * 2a - vf^2
			//vf = peak velocity
			double vf = (targetValue - initValue) * 2.0 * acc + initVelocity * initVelocity;
			vf = vf / 2.0;
			vf = std::sqrt(vf);
			vf = vf * expectedV / std::abs(expectedV);
			double AcceleratingPhaseDuration = (vf - initVelocity) / acc;
			double breakValue = initValue + initVelocity * AcceleratingPhaseDuration
				+ 1.0 / 2.0 * acc * AcceleratingPhaseDuration * AcceleratingPhaseDuration;
			double EndPhaseDuration = (0 - vf) / -acc;
			if (time < AcceleratingPhaseDuration)	
			{
				return initVelocity + acc  * time;
			}
			else if (time < AcceleratingPhaseDuration + EndPhaseDuration)
			{
				time -= AcceleratingPhaseDuration;
				return vf - acc * time;
			}
			else
			{
				return 0.0;
			}
		}
	}
	double Smooth::getValueAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		double initVelocity = property.getLastVelocity();
		double initValue = property.getLastValue();
		double targetValue = property.getTargetValue();
		double acc = mPropertyList[ParameterList::Accelerator];
		double expectedV = mPropertyList[ParameterList::ExpectedVelocity];
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		//vf^2 - vi^2 = 2 a s;
		double expectedDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
			/ (2.0 * acc);
		double expectedFinalDistance = (0 - (expectedV * expectedV))
			/ (2.0 * -acc);
		//normal case
		if (std::abs(expectedDistance + expectedFinalDistance) < std::abs(targetValue - initValue))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
				/ (2.0 * acc);
			double finalDistance = (0 - (expectedV * expectedV))
				/ (2.0 * -acc);
			double EndPhaseDuration = (0 - expectedV) / -acc;
			double maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance;
			double MaintainPhaseDuration = maintainPhaseDistance / expectedV;
			if (time < AcceleratingPhaseDuration)
			{
				return initValue + initVelocity * time + 1.0 / 2.0 * acc * time * time;
			}
			else if (time < AcceleratingPhaseDuration + MaintainPhaseDuration)
			{
				time -= AcceleratingPhaseDuration;
				return initValue + AcceleratingDistance + time * expectedV;
			}
			else if (time < AcceleratingPhaseDuration + MaintainPhaseDuration + EndPhaseDuration)
			{
				time -= MaintainPhaseDuration;
				time -= AcceleratingPhaseDuration;
				return initValue + AcceleratingDistance + maintainPhaseDistance + expectedV * time + 1.0 / 2.0 * -acc * time * time;
			}
			else
			{
				return targetValue;
			}
		}
		else
		{
			//vf^2 - vi^2 = (target -init) * 2a - vf^2
			//vf = peak velocity
			double vf = (targetValue - initValue) * 2.0 * acc + initVelocity * initVelocity;
			vf = vf / 2.0;
			vf = std::sqrt(vf);
			vf = vf * expectedV / std::abs(expectedV);
			double AcceleratingPhaseDuration = (vf - initVelocity) / acc;
			double breakValue = initValue + initVelocity * AcceleratingPhaseDuration
				+ 1.0 / 2.0 * acc * AcceleratingPhaseDuration * AcceleratingPhaseDuration;
			double EndPhaseDuration = (0 - vf) / -acc;
			if (time < AcceleratingPhaseDuration)
			{
				return initValue + initVelocity * time + 1.0 / 2.0 * acc * time * time;
			}
			else if (time < AcceleratingPhaseDuration + EndPhaseDuration)
			{
				time -= AcceleratingPhaseDuration;
				return breakValue + vf * time + 1.0 / 2.0 * -acc * time * time;
			}
			else
			{
				return targetValue;
			}
		}
	}
}