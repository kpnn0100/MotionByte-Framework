#include "../PropertyHeader.h"
#include <math.h>
namespace MotionByte
{
	inline bool Smooth::isHavingSustainPhase(double initV2, double expectedV2, double initValue, double finalValue, double acc)
	{
		//vf^2 - vi^2 = 2 a s;
		double expectedDistance = (expectedV2 - initV2)
			/ (2.0 * acc);
		double expectedFinalDistance = (0 - expectedV2)
			/ (2.0 * -acc);
		return std::abs(expectedDistance + expectedFinalDistance) < std::abs(finalValue - initValue);
	}
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
		double expectedVPow2 = std::pow(expectedV, 2);
		double initVelocityPow2 = std::pow(initVelocity, 2);
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		//normal case
		if (isHavingSustainPhase(initVelocityPow2, expectedVPow2, initValue, targetValue, acc))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = (std::pow(expectedV, 2) - std::pow(initVelocity, 2))
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
		double expectedVPow2 = std::pow(expectedV, 2);
		double initVelocityPow2 = std::pow(initVelocity, 2);
		if (targetValue == initValue)
		{
			return 0.0;
		}
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		//normal case
		if (isHavingSustainPhase(initVelocityPow2, expectedVPow2, initValue, targetValue, acc))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = ((expectedV * expectedV) - (initVelocity * initVelocity))
				/ (2 * acc);
			double finalDistance = (0 - (expectedV * expectedV))
				/ (2.0 * -acc);
			double EndPhaseDuration = (0 - expectedV) / -acc;
			double maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance;
			double MaintainPhaseDuration = maintainPhaseDistance / expectedV;
			// Speed
			//accelerating phase
			double velocity = (time < AcceleratingPhaseDuration) ?
				(initVelocity + acc * time) : 0.0;
			time -= AcceleratingPhaseDuration;
			// Maintaining Phase
			velocity += (time >= 0 && time < MaintainPhaseDuration) ?
				(expectedV) : 0.0;
			time -= MaintainPhaseDuration;
			// End Phase
			velocity += (time >= 0 && time < EndPhaseDuration) ?
				(expectedV + 2.0 * -acc * time) : 0.0;
			return velocity;
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
		double expectedVPow2 = std::pow(expectedV, 2);
		double initVelocityPow2 = std::pow(initVelocity, 2);
		if (targetValue < initValue)
		{
			expectedV = -expectedV;
			acc = -acc;
		}
		//normal case
		if (isHavingSustainPhase(initVelocityPow2, expectedVPow2, initValue, targetValue,acc))
		{
			double AcceleratingPhaseDuration = (expectedV - initVelocity) / acc;
			double AcceleratingDistance = (expectedVPow2 - initVelocityPow2)
				/ (2.0 * acc);
			double finalDistance = (0 - expectedVPow2)
				/ (2.0 * -acc);
			double EndPhaseDuration = (0 - expectedV) / -acc;
			double maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance;
			double MaintainPhaseDuration = maintainPhaseDistance / expectedV;
			// Accelerating Phase
			double position = (time < AcceleratingPhaseDuration) ?
				(initValue + initVelocity * time + 0.5 * acc * time * time) : 0.0;
			time -= AcceleratingPhaseDuration;
			// Maintaining Phase
			position += (time >= 0 && time < MaintainPhaseDuration) ?
				(initValue + AcceleratingDistance + time * expectedV) : 0.0;
			time -= MaintainPhaseDuration;
			// End Phase
			position += (time >= 0 && time < EndPhaseDuration) ?
				(initValue + AcceleratingDistance + maintainPhaseDistance +
					expectedV * time +
					0.5 * -acc * time * time ) : 0.0;
			time -= EndPhaseDuration;
			// Final Phase
			position += (time >= 0.0) ? targetValue : 0.0;
			return position;

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

			double position = (time >= 0 && time < AcceleratingPhaseDuration) ?
				(initValue + initVelocity * time + 0.5 * acc * time * time) : 0.0;
			time -= AcceleratingPhaseDuration;
			// End Phase
			position += (time >= 0 && time < EndPhaseDuration) ?
				(breakValue + vf * time + 1.0 / 2.0 * -acc * time * time) : 0.0;
			time -= EndPhaseDuration;
			// Final Phase
			position += (time >= 0.0) ? targetValue : 0.0;
			return position;
		}
	}
}