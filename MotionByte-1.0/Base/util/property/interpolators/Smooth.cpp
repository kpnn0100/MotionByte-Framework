#include "../PropertyHeader.h"
#include <animator/timer/Timer.h>
#include <math.h>

namespace MotionByte
{
	// base function : x = v0*t + 1/2*a*t^2

	Timer timer(true);
	inline bool Smooth::isHavingSustainPhase(double initV, double expectedV, double initValue, double finalValue, double acc)
	{
		double t_indepent = (expectedV - initV) / acc;
		double AcceleratingDistance = initV * t_indepent + 1.0 / 2.0 * acc * std::pow(t_indepent, 2);
		double t_indepent2 = expectedV / acc;
		double finalDistance = 1.0 / 2.0 * acc * std::pow(t_indepent2, 2);
		return std::abs(AcceleratingDistance + finalDistance) < std::abs(finalValue - initValue);
	}
	inline bool Smooth::isHavingAcceleratingPhase(double initV, double expectedV, double initValue, double finalValue, double acc)
	{
		if ((finalValue-initValue)*initV <= 0.0)
		{
			return true;
		}
		// v1^2 - v0^2 = 2as
		double expectedV2 = 0;
		double initV2 = std::pow(initV, 2);
		double expectedV2_initV2 = expectedV2 - initV2;
		double distance_needed_to_slow_down = expectedV2_initV2 / (2 * acc);
		debug(3, "distance_needed_to_slow_down:", distance_needed_to_slow_down);
		if (distance_needed_to_slow_down > 0)
		{
			if (distance_needed_to_slow_down < finalValue - initValue)
			{
				return true;
			}
			return false;
		} else
		{
			if (distance_needed_to_slow_down > finalValue - initValue)
			{
				return true;
			}
			return false;
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
			state[TargetVelocity] = -state[TargetVelocity];
			state[Acceleration] = -state[Acceleration];
		}
		state[IsHavingAcceleratingPhase] = 1.0 * isHavingAcceleratingPhase(state[InitVelocity], state[TargetVelocity],
																 state[InitValue], state[TargetValue], state[Acceleration]);
		state[IsHavingSustainPhase] = 1.0 * isHavingSustainPhase(state[InitVelocity], state[TargetVelocity],
																 state[InitValue], state[TargetValue], state[Acceleration]);
		if (state[IsHavingAcceleratingPhase] == 0.0)
		{
			state[AcceleratingDuration] = 0.0;
			state[AcceleratingDistance] = 0.0;
			state[MaintainingDistance] = 0.0;
			state[MaintainingDuration] = 0.0;
			// calculate acc needed to reach target value
			// v1^2 - v0^2 = 2as
			// 0 - state[InitVelocity]^2 = 2 * state[Acceleration] * distanceToGo
			state[Acceleration] = -state[InitVelocity]*state[InitVelocity] / (2.0 * distanceToGo);
			state[MiddleVelocity] = state[InitVelocity];
			// 0 = v1 - a*t => t = v1/a
			state[DeceleratingDuration] = state[MiddleVelocity] / state[Acceleration];
			state[DeceleratingDistance] = distanceToGo;
			return;
		}
		if (state[IsHavingSustainPhase] == 1.0)
		{
			// v1 = v0 + a*t -> t = (v1 - v0) / a
			state[AcceleratingDuration] = (state[TargetVelocity] - state[InitVelocity]) / state[Acceleration];
			state[AcceleratingDistance] = state[InitVelocity] * state[AcceleratingDuration] + 1.0 / 2.0 * state[Acceleration] * std::pow(state[AcceleratingDuration], 2);
			state[DeceleratingDuration] = state[TargetVelocity] / state[Acceleration];
			state[DeceleratingDistance] = state[TargetVelocity] * state[DeceleratingDuration] -  1.0 / 2.0 * state[Acceleration] * std::pow(state[DeceleratingDuration], 2);
			state[MaintainingDistance] = state[TargetValue] - state[InitValue]- state[AcceleratingDistance] - state[DeceleratingDistance];
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
			(
					std::sqrt(2)
					*std::sqrt(
						2*state[Acceleration]*distanceToGo
						+ std::pow(state[InitVelocity],2)
					)
				- 2*state[InitVelocity]
			) 
			/ (2*state[Acceleration]);

			if (state[AcceleratingDuration] < 0.0)
			{
				state[AcceleratingDuration] =
				(
						-std::sqrt(2)
						*std::sqrt(
							2*state[Acceleration]*distanceToGo
							+ std::pow(state[InitVelocity],2)
						)
					- 2*state[InitVelocity]
				) / (2*state[Acceleration]);
			}
			state[AcceleratingDistance] = state[InitVelocity] * state[AcceleratingDuration] + 1.0 / 2.0 * state[Acceleration] * std::pow(state[AcceleratingDuration], 2);
			state[DeceleratingDistance] = distanceToGo - state[AcceleratingDistance];
			state[MiddleVelocity] = state[InitVelocity] + state[Acceleration] * state[AcceleratingDuration];
			// 0 = v1 - a*t => t = v1/a
			state[DeceleratingDuration] = state[MiddleVelocity] / state[Acceleration];
			state[MaintainingDistance] = 0.0;
			state[MaintainingDuration] = 0.0;

		}
		if (property.getName() == "slider::percent")
			{
			debug(3, property.getName(), ",time:", time, 
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
	}
	bool Smooth::isSet(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		if (time >= state[AcceleratingDuration] + state[MaintainingDuration] + state[DeceleratingDuration])
		{
			
			return true;
		}
		return false;
	}
	double Smooth::getVelocityAtTime(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		double velocity = 0.0;
		if (time < state[AcceleratingDuration])
		{
			velocity =  state[InitVelocity]  + state[Acceleration] * time;
		}

		time -= state[AcceleratingDuration];

		// Maintaining Phase
		if (0 <= time && time < state[MaintainingDuration])
		{
			velocity = state[MiddleVelocity];
		}
		time -= state[MaintainingDuration];

		// End Phase
		if (0 <= time && time < state[DeceleratingDuration])
		{
			velocity = state[MiddleVelocity] - state[Acceleration] * time;
		}
		time -= state[DeceleratingDuration];

		// Final Phase
		if (time >= 0.0)
		{
			velocity = 0.0;
		}
		return velocity;
	}
	double Smooth::getValueAtTime(Property &property)
	{
		double time = property.getElapsedTime();
		auto &state = property.getInterpolatorState();
		double position = 0.0;
				if (property.getName() == "slider::percent")
			{
			debug(3, property.getName(), 
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
		debug(3,property.getName(), "start phase", "time:", time, "position:", position);
		if (time >= 0.0 && time < state[AcceleratingDuration])
		{
			position = state[InitValue] + state[InitVelocity] * time + 1.0 / 2.0 * state[Acceleration] * std::pow(time, 2);
			if (property.getName() == "slider::percent")
			{
				debug(3,property.getName(), "acc phase", "time:", time, "position:", position);
			}
		}

		time -= state[AcceleratingDuration];

		// Maintaining Phase
		if (0.0 <= time && time < state[MaintainingDuration])
		{
			position = state[InitValue] + state[AcceleratingDistance] + state[MiddleVelocity] * time;
			if (property.getName() == "slider::percent")
			{
				debug(3,property.getName(), "maintaining phase", "time:", time, "position:", position);
			}
		}
		time -= state[MaintainingDuration];

		// End Phase
		if (0.0 <= time && time < state[DeceleratingDuration])
		{
			position = state[InitValue] + state[AcceleratingDistance] + state[MaintainingDistance]
						+ state[MiddleVelocity] * time
						- 1.0 / 2.0 * state[Acceleration] * std::pow(time, 2);
			if (property.getName() == "slider::percent")
			{
				debug(3,property.getName(), "decce phase", "time:", time, "position:", position);
			}
		}
		time -= state[DeceleratingDuration];

		// Final Phase
		if (time >= 0.0)
		{
			if (property.getName() == "slider::percent")
			{
				debug(3,property.getName(), "end phase", "time:", time, "position:", position);
			}
			position = state[TargetValue];
		}
		if (property.getName() == "slider::percent")
		{
			debug(3,property.getName(), ":time:", time
						,",init :", state[InitValue]
						,",position:", position
						,",having sustain phase:", state[IsHavingSustainPhase] ? "true" : "false"
						,",middle velocity:", state[MiddleVelocity]
						, ",target"
						,state[TargetValue]);
		}
		return position;
	}
}