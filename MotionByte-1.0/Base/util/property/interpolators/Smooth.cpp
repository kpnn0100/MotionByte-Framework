#include "../PropertyHeader.h"
#include <math.h>
namespace MotionByte
{
	inline bool Smooth::isHavingSustainPhase(double initV, double expectedV, double initValue, double finalValue, double acc)
	{
		double t_indepent = std::sqrt((expectedV - initV) / acc);
		double AcceleratingDistance = initV * t_indepent + 1.0 / 3.0 * acc * std::pow(t_indepent, 3);
		double t_indepent2 = std::sqrt(expectedV / acc);
		double finalDistance = 1.0 / 3.0 * acc * std::pow(t_indepent2, 3);
		return std::abs(AcceleratingDistance + finalDistance) < std::abs(finalValue - initValue);

	}
	Smooth::Smooth(double accelerator, double expectedVelocity) : Interpolator(ParameterCount)
	{
		mPropertyList[ParameterList::Accelerator] = accelerator;
		mPropertyList[ParameterList::ExpectedVelocity] = expectedVelocity;
	}
	void Smooth::updateStateFor(Property& property)
	{
		auto& state = property.getInterpolatorState();
		state.resize(SizeOfState);
		state[InitVelocity] = property.getLastVelocity();
		state[InitValue] = property.getLastValue();
		state[TargetValue] = property.getTargetValue();
		state[Acceleration] = mPropertyList[ParameterList::Accelerator];
		state[TargetVelocity] = mPropertyList[ParameterList::ExpectedVelocity];
		if (state[TargetValue] < state[InitValue]) {
			state[TargetVelocity] = -state[TargetVelocity];
			state[Acceleration] = -state[Acceleration];
		}
		state[IsHavingSustainPhase] = 1.0
			* isHavingSustainPhase(state[InitVelocity], state[TargetVelocity],
				state[InitValue], state[TargetValue], state[Acceleration]);
		state[OffsetDistance] = 0.0;
		state[OffsetDuration] = 0.0;
		if (state[InitVelocity] != 0.0) {
			state[OffsetDuration] = -std::pow(std::abs(state[InitVelocity] / state[Acceleration]), 1.0 / 2.0)
				* state[InitVelocity] / std::abs(state[InitVelocity])
				* state[Acceleration] / std::abs(state[Acceleration]);
		}
		state[OffsetDistance] = -1.0 / 3.0 * state[Acceleration] * std::pow(state[OffsetDuration], 3);
		if (state[InitVelocity] != 0.0) {
			state[OffsetDistance] = state[OffsetDistance] * state[InitVelocity] / std::abs(state[InitVelocity]) * state[Acceleration] / std::abs(state[Acceleration]);
		}
		if (state[IsHavingSustainPhase] == 1.0)
		{
			double start_position = state[InitValue] - state[OffsetDistance];

			state[AcceleratingDuration] = std::sqrt((state[TargetVelocity]) / state[Acceleration]);
			double t_indepent = std::sqrt((state[TargetVelocity]) / state[Acceleration]);
			state[AcceleratingDistance] = 1.0 / 3.0 * state[Acceleration] * std::pow(t_indepent, 3);
			double t_indepent2 = std::sqrt(state[TargetVelocity] / state[Acceleration]);
			
			state[DeceleratingDistance] = 1.0 / 3.0 * state[Acceleration] * std::pow(t_indepent2, 3);
			state[DeceleratingDuration] = t_indepent2;
			state[MaintainingDistance] = state[TargetValue]+ state[OffsetDistance] - state[InitValue] - state[AcceleratingDistance] - state[DeceleratingDistance];
			state[MaintainingDuration] = state[MaintainingDistance] / state[TargetVelocity];
			
		}
		else
		{
			double start_position = state[InitValue] - state[OffsetDistance];
			state[AcceleratingDistance] = (state[TargetValue] - start_position) / 2.0;
			state[AcceleratingDuration] = std::pow((state[AcceleratingDistance] * 3.0 / state[Acceleration]), 1.0 / 3.0);
			if ( std::isinf(state[AcceleratingDuration]))
			{
				int breakhere = 1;
			}
		}
	}
	bool Smooth::isSet(Property& property)
	{
		double time = property.getElapsedTime();
		auto& state = property.getInterpolatorState();

		time -= state[OffsetDuration];
		if (state[IsHavingSustainPhase] == 1.0) {
			time -= state[AcceleratingDuration];
			time -= state[MaintainingDuration];
			time -= state[DeceleratingDuration];

			// Final Phase
			if (time >= 0.0) {
				return true;
			}
			return false;
		}
		else {
			time -= state[AcceleratingDuration]*2;

			// Final Phase
			if (time >= 0.0) {
				return true;
			}
			return false;
		}
	}
	double Smooth::getVelocityAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		auto& state = property.getInterpolatorState();
		double velocity = 0.0;
		if (time < state[OffsetDuration]) {
			velocity = state[Acceleration] * std::pow((state[OffsetDuration] - time), 2);
		}
		time -= state[OffsetDuration];
		if (state[IsHavingSustainPhase] == 1.0) {
			if (time < state[AcceleratingDuration]) {
				velocity = state[Acceleration] * std::pow(time, 2);
			}
			time -= state[AcceleratingDuration];

			// Maintaining Phase
			if (0 <= time && time < state[MaintainingDuration]) {
				velocity = state[TargetVelocity];
			}
			time -= state[MaintainingDuration];

			// End Phase
			if (0 <= time && time < state[DeceleratingDuration]) {
				velocity = state[Acceleration] * std::pow((time - state[DeceleratingDuration]), 2);
			}
			time -= state[DeceleratingDuration];

			// Final Phase
			if (time >= 0.0) {
				velocity = 0.0;
			}

			return velocity;
		}
		else {
			// Sustain Phase

			if (0 <= time && time < state[AcceleratingDuration]) {
				velocity = state[Acceleration] * std::pow(time, 2);
			}
			time -= state[AcceleratingDuration];

			// End Phase
			if (0 <= time && time < state[AcceleratingDuration]) {
				velocity = state[Acceleration] * std::pow((time - state[AcceleratingDuration]), 2.0);
			}
			time -= state[AcceleratingDuration];

			// Final Phase
			if (time >= 0.0) {
				velocity = 0.0;
			}
			return velocity;
		}
	}
	double Smooth::getValueAtTime(Property& property)
	{
		double time = property.getElapsedTime();
		auto& state = property.getInterpolatorState();
		double position = 0.0;
		debug("state");
		if (time < state[OffsetDuration]) {
			position = (state[InitValue] - state[OffsetDistance] + 1.0 / 3.0 * state[Acceleration] * std::pow((state[OffsetDuration] - time), 3));
			debug("OffsetDuration");
			debug(position);
			return position;
		}
		time -= state[OffsetDuration];

		if (state[IsHavingSustainPhase] == 1.0) {

			if (time < state[AcceleratingDuration]) {
				debug("AcceleratingDuration");
				position = state[InitValue] - state[OffsetDistance] + 1.0 / 3.0 * state[Acceleration] * std::pow(time, 3);
			}
			time -= state[AcceleratingDuration];

			// Maintaining Phase
			if (0 <= time && time < state[MaintainingDuration]) {
				debug("MaintainingDuration");
				position = state[InitValue] - state[OffsetDistance] + state[AcceleratingDistance] + state[TargetVelocity]* time;
			}
			time -= state[MaintainingDuration];

			// End Phase
			if (0 <= time && time < state[DeceleratingDuration]) {
				debug("DeceleratingDuration");
				position = state[InitValue] - state[OffsetDistance]
					+ state[AcceleratingDistance]
					+ state[MaintainingDistance]
					+ state[DeceleratingDistance]
					+ 1.0 / 3.0 * state[Acceleration]
					* std::pow((time - state[DeceleratingDuration]), 3);
			}
			time -= state[DeceleratingDuration];

			// Final Phase
			if (time >= 0.0) {
				debug("Final");
				position = state[TargetValue];
			}
		}
		else {
			// Sustain Phase
			if (0 <= time && time < state[AcceleratingDuration]) {
				position = state[InitValue] - state[OffsetDistance] + 1.0 / 3.0 * state[Acceleration] * std::pow(time, 3);
				debug("AcceleratingDuration");
			}
			time -= state[AcceleratingDuration];

			// End Phase
			if (0 <= time && time < state[AcceleratingDuration]) {
				position = state[InitValue] - state[OffsetDistance] + state[AcceleratingDistance] * 2.0 + 1.0 / 3.0 * state[Acceleration] * std::pow((time - state[AcceleratingDuration]), 3.0);
				debug("DeceleratingDuration");
			}
			time -= state[AcceleratingDuration];

			// Final Phase
			if (time >= 0.0) {
				position = state[TargetValue];
			}
		}
		debug(position);
		return position;
	}
}