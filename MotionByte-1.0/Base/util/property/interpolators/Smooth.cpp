#include "../PropertyHeader.h"
#include <animator/timer/Timer.h>
#include <math.h>

namespace MotionByte
{
	// base function : x = v0*t + 1/2*a*t^2
	/**
	 * let's trace from the destination, because it should reach the destination with velocity 0
	 * x0 ------ x1
	 * x1 is the destination with velocity 0
	 * distance needed to reach velocity 0 from expected velocity is v0^2/(2*a)
	 * 2 cases:
	 * 	1. Destination can be reached with default acceleration
	 *    1.1. there are mainting phase
	 *    1.2. there are no maintaining phase
	 * 	2. Destination can't be reached with default acceleration
	 */
	Timer timer(true);
	inline bool canReachDesinationWithDefaultAcc(double velocity, double initValue, double finalValue, double acc)
	{
		double distanceToGo = finalValue - initValue;
		// v1^2 - v0^2 = 2as
		double distanceToReachV0 = velocity*velocity / (2 * acc);
		if (distanceToGo < 0)
		{
			return distanceToGo <= distanceToReachV0;
		}
		else
		{
			return distanceToGo >= distanceToReachV0;
		}
	}
	inline bool isHavingMaintainingPhase(double initV, double expectedV, double initValue, double finalValue, double acc)
	{
		double distanceToGo = finalValue - initValue;
		double distanceToReachV1 = (expectedV*expectedV-initV*initV) / (2 * acc);
		double distanceToStopFromV1 = expectedV*expectedV / (2 * acc);
		debug(DebugLevel::Verbose, "distanceToGo:", distanceToGo
			, ",distanceToReachV1:", distanceToReachV1, ",distanceToStopFromV1:", distanceToStopFromV1
			, ",initV:", initV, ",expectedV:", expectedV, ",initValue:", initValue, ",finalValue:", finalValue, ",acc:", acc);
		if (distanceToGo < 0)
		{
			return distanceToGo <= distanceToReachV1 + distanceToStopFromV1;
		}
		else
		{
			return distanceToGo >= distanceToReachV1 + distanceToStopFromV1;
		}
	}

	InterpolatorModule Smooth::create(double accelerator, double expectedVelocity)
	{
		InterpolatorModule smooth(InterpolatorType::SMOOTH, ParameterList::ParameterCount);
		smooth.getPropertyList()[ParameterList::Accelerator] = accelerator;
		smooth.getPropertyList()[ParameterList::ExpectedVelocity] = expectedVelocity;
		return smooth;
	}
	void Smooth::updateStateFor(Property &property)
	{
		auto &state = property.getInterpolatorState();
		state.resize(SizeOfState);
		state[InitVelocity] = property.getLastVelocity();
		state[InitValue] = property.getLastValue();
		state[TargetValue] = property.getTargetValue();
		state[Acceleration] = property.getInterpolator().getPropertyList()[ParameterList::Accelerator];
		state[TargetVelocity] = property.getInterpolator().getPropertyList()[ParameterList::ExpectedVelocity];
		double distanceToGo = state[TargetValue] - state[InitValue];
		if (state[TargetValue] < state[InitValue])
		{
			state[Acceleration] = -state[Acceleration];
			state[TargetVelocity] = -state[TargetVelocity];
		}

		if (canReachDesinationWithDefaultAcc(state[InitVelocity], state[InitValue], state[TargetValue], state[Acceleration]))
		{
			debug(DebugLevel::Verbose,property.getName(),"can reach destination with default acceleration");
			// v1^2 - v0^2 = 2as
			if (isHavingMaintainingPhase(state[InitVelocity], state[TargetVelocity], state[InitValue], state[TargetValue], state[Acceleration]))
			{
				debug(DebugLevel::Verbose,property.getName(),"have maintaining phase");
				state[AcceleratingDuration] = (state[TargetVelocity] - state[InitVelocity]) / state[Acceleration];
				state[AcceleratingDistance] = state[InitVelocity] * state[AcceleratingDuration] + 0.5 * state[Acceleration] * std::pow(state[AcceleratingDuration],2);
				state[DeceleratingDuration] =  state[TargetVelocity] / state[Acceleration];
				state[DeceleratingDistance] = state[TargetVelocity] * state[DeceleratingDuration] - 0.5 * state[Acceleration] * std::pow(state[DeceleratingDuration],2);
				state[MaintainingDistance] = distanceToGo - state[AcceleratingDistance] - state[DeceleratingDistance];
				state[MaintainingDuration] = state[MaintainingDistance] / state[TargetVelocity];
				state[MiddleVelocity] = state[TargetVelocity];
			}
			else
			{
				/**
				 * c-----------------d
						 r
				x0       x1       x2

				x1 = x0 + v0 * t1 + 1/2 a t1^2
				x2 = x1 + v1 * t2 - 1/2 a t2^2

				x2 = x0 + v0 * t1 + 1/2 a t1^2 + v1 * t2 - 1/2 a t2^2
				d - c = v0 * t1 + 1/2 a t1^2 + v1 * t2 - 1/2 a t2^2
				d - c = v0 * t1 + 1/2 a t1^2 + (v0 + at1) * (v0/a + t1) - 1/2 a (v0/a + t1)^2
				0 = v1 -at2
				v1 = v0 + at1
				v0 + at1-at2=0
				v0/a + t1 = t2
				t1 = (signOf(d-c) sqrt(2) * sqrt(2*a*(d-c) + v0^2) - 2*v0)/(2*a)
				*/
				state[AcceleratingDuration] = 
							(std::sqrt(2)
							* std::sqrt(2 * state[Acceleration] * distanceToGo + std::pow(state[InitVelocity], 2))
						- 2 * state[InitVelocity])
					/ (2 * state[Acceleration]);
				if (state[AcceleratingDuration] < 0.0)
				{
					state[AcceleratingDuration] = 
							(-std::sqrt(2)
							* std::sqrt(2 * state[Acceleration] * distanceToGo + std::pow(state[InitVelocity], 2))
						- 2 * state[InitVelocity])
					/ (2 * state[Acceleration]);
				}
				state[MiddleVelocity] = state[InitVelocity] + state[Acceleration] * state[AcceleratingDuration];
				state[AcceleratingDistance]	= state[InitVelocity] * state[AcceleratingDuration]
					+ 0.5 * state[Acceleration] * std::pow(state[AcceleratingDuration], 2);
				state[MaintainingDuration] = 0;
				state[MaintainingDistance] = 0;
				state[DeceleratingDistance] = distanceToGo - state[AcceleratingDistance];
				state[DeceleratingDuration] = state[MiddleVelocity] / state[Acceleration];
			}
		}
		else // can not reach destination with default acceleration
		{
			// v1^2 - v0^2 = 2as
			debug(DebugLevel::Verbose,property.getName(),"can not reach destination with default acceleration");
			state[Acceleration] = std::pow(state[InitVelocity], 2)
									/ (2 * distanceToGo);
			state[AcceleratingDuration] = 0.0;
			state[AcceleratingDistance] = 0.0;
			state[MaintainingDuration] = 0.0;
			state[MaintainingDistance] = 0.0;
			state[DeceleratingDuration] = state[InitVelocity] / state[Acceleration];
			state[DeceleratingDistance] = state[TargetValue] - state[InitValue];
			state[MiddleVelocity] = state[InitVelocity];
		}
		debug(DebugLevel::Verbose, property.getName(), 
						",IsHavingAcceleratingPhase:", state[IsHavingAcceleratingPhase],
						",IsHavingSustainPhase:", state[IsHavingSustainPhase],
						",InitVelocity:", state[InitVelocity],
						",MiddleVelocity:", state[MiddleVelocity],
						",TargetVelocity:", state[TargetVelocity],
						",InitValue:", state[InitValue],
						",TargetValue:", state[TargetValue],
						",Acceleration:", state[Acceleration],
						",AcceleratingDuration:", state[AcceleratingDuration],
						",AcceleratingDistance:", state[AcceleratingDistance],
						",DeceleratingDuration:", state[DeceleratingDuration],
						",DeceleratingDistance:", state[DeceleratingDistance],
						",MaintainingDuration:", state[MaintainingDuration],
						",MaintainingDistance:", state[MaintainingDistance]);
	}
	bool Smooth::isSet(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		if (time > state[AcceleratingDuration] + state[DeceleratingDuration] + state[MaintainingDuration])
		{
			return true;
		}
		return false;
	}
	double Smooth::getVelocityAtTime(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		if (time < state[AcceleratingDuration])
		{
			return state[InitVelocity] + state[Acceleration] * time;
		}
		time = time - state[AcceleratingDuration];
		if (time < state[MaintainingDuration])
		{
			return state[TargetVelocity];
		}
		time = time - state[MaintainingDuration];
		if (time < state[DeceleratingDuration])
		{
			return state[MiddleVelocity] - state[Acceleration] * time;
		}
		return 0;
	}
	double Smooth::getValueAtTime(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		if (time < state[AcceleratingDuration])
		{
			return state[InitValue] + state[InitVelocity] * time + 0.5 * state[Acceleration] * std::pow(time, 2);
		}
		time = time - state[AcceleratingDuration];
		if (time < state[MaintainingDuration])
		{
			return state[InitValue] + state[AcceleratingDistance] + state[TargetVelocity] * time;
		}
		time = time - state[MaintainingDuration];
		if (time < state[DeceleratingDuration])
		{
			return state[InitValue] + state[AcceleratingDistance] + state[MaintainingDistance] + state[MiddleVelocity]*time - 0.5 * state[Acceleration] * std::pow(time, 2);
		}
		return state[TargetValue];
	}
}