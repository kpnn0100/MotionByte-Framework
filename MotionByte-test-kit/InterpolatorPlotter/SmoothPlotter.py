import matplotlib.pyplot as plt
import numpy as np

import math
def isHavingSustainPhase(initV, expectedV, initValue, finalValue, acc):
    t_indepent = np.sqrt((expectedV-initV)/acc)
    AcceleratingDistance = initV*t_indepent + 1/3 *acc*t_indepent**3
    t_indepent2 = np.sqrt((expectedV-0)/acc)
    finalDistance = 1/3 *acc *t_indepent2**3
    return np.abs(AcceleratingDistance+ finalDistance)<np.abs(finalValue-initValue)
#s = vi * sqrt((vf-vi)/a) + 1/3 ((vf-vi)/a)**1.5
def calculate_speed(time,initValue,targetValue,initVelocity, expectedV,acc):
    if targetValue < initValue:
        expectedV = -expectedV
        acc = -acc
    if (isHavingSustainPhase(initVelocity,expectedV,initValue,targetValue,acc)):
        # Accelerating Phase
        AcceleratingPhaseDuration = np.sqrt((expectedV - initVelocity) / acc)
        t_indepent = np.sqrt((expectedV-initVelocity)/acc)
        AcceleratingDistance = initVelocity*t_indepent + 1/3 *acc*t_indepent**3
        t_indepent2 = np.sqrt((expectedV-0)/acc)
        finalDistance = 1/3 *acc *t_indepent2**3
        EndPhaseDuration = t_indepent2
        maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance
        MaintainPhaseDuration = maintainPhaseDistance / expectedV

        velocity = 0.0

        if time < AcceleratingPhaseDuration:
            velocity =  initVelocity + acc * time**2
        time -= AcceleratingPhaseDuration

        # Maintaining Phase
        if 0 <= time < MaintainPhaseDuration:
            velocity = expectedV
        time -= MaintainPhaseDuration

        # End Phase
        if 0 <= time < EndPhaseDuration:
            velocity = acc * (time-EndPhaseDuration)**2
        time -= EndPhaseDuration

        # Final Phase
        if time >= 0.0:
            velocity += 0.0

        return velocity
    else:
        #accelarating_distance = full_distance - deaccelarating_distance
        #vi * sqrt((vf-vi)/a) + 1/3 ((vf-vi)/a)**1.5 = (target - init) - (1/3 ((vf)/a)**1.5)
        offsetDistance = 0.0
        #suppose it start a while before
        offset_time = 0
        if (initVelocity != 0.0):
            offset_time = -(np.abs(initVelocity/acc))**(1/3) * initVelocity/np.abs(initVelocity) * acc/np.abs(acc)
        offsetDistance =  -1/3 *acc* offset_time**3
        if (initVelocity != 0.0):
            offsetDistance = offsetDistance * initVelocity/np.abs(initVelocity) * acc/np.abs(acc)
        start_position = initValue-offsetDistance
        accelerating_distance = (targetValue-start_position)/2.0
        accelerating_duration = (accelerating_distance*3/acc)**(1/3)
        

        velocity = 0.0
        if time < offset_time:
            velocity = -acc*(offset_time-time)**2
        time -= offset_time
        if 0<= time < accelerating_duration:
            velocity = acc * time**2
        time -= accelerating_duration
        # End Phase
        if 0 <= time < accelerating_duration:
            velocity =  acc * (time-accelerating_duration)**2
        time -= accelerating_duration

        # Final Phase
        if time >= 0.0:
            velocity = 0.0

        return velocity
def calculate_position(time,initValue,targetValue,initVelocity, expectedV,acc):
    if targetValue < initValue:
        expectedV = -expectedV
        acc = -acc
    if (isHavingSustainPhase(initVelocity,expectedV,initValue,targetValue,acc)):
        # Accelerating Phase
        AcceleratingPhaseDuration = np.sqrt((expectedV - initVelocity) / acc)
        t_indepent = np.sqrt((expectedV-initVelocity)/acc)
        AcceleratingDistance = initVelocity*t_indepent + 1/3 *acc*t_indepent**3
        t_indepent2 = np.sqrt((expectedV-0)/acc)
        finalDistance = 1/3 *acc *t_indepent2**3
        EndPhaseDuration = t_indepent2
        maintainPhaseDistance = targetValue - initValue - AcceleratingDistance - finalDistance
        MaintainPhaseDuration = maintainPhaseDistance / expectedV

        position = 0.0

        if time < AcceleratingPhaseDuration:
            position = initValue + initVelocity * time + 1/3 * acc * time**3
        time -= AcceleratingPhaseDuration

        # Maintaining Phase
        if 0 <= time < MaintainPhaseDuration:
            position = initValue + AcceleratingDistance + time * expectedV
        time -= MaintainPhaseDuration

        # End Phase
        if 0 <= time < EndPhaseDuration:
            position = initValue + AcceleratingDistance + maintainPhaseDistance+finalDistance +  1/3 * acc * (time-EndPhaseDuration)**3
        time -= EndPhaseDuration

        # Final Phase
        if time >= 0.0:
            position = targetValue

        return position
    else:
        #accelarating_distance = full_distance - deaccelarating_distance
        #vi * sqrt((vf-vi)/a) + 1/3 ((vf-vi)/a)**1.5 = (target - init) - (1/3 ((vf)/a)**1.5)
        offsetDistance = 0.0
        #suppose it start a while before
        offset_time = 0.0
        if (initVelocity != 0.0):
            offset_time = -(np.abs(initVelocity/acc))**(1/3) * initVelocity/np.abs(initVelocity) * acc/np.abs(acc)
        offsetDistance =  -1/3 *acc* offset_time**3
        if (initVelocity != 0.0):
            offsetDistance = offsetDistance * initVelocity/np.abs(initVelocity) * acc/np.abs(acc)
        start_position = initValue-offsetDistance
        accelerating_distance = (targetValue-start_position)/2.0
        accelerating_duration = (accelerating_distance*3/acc)**(1/3)
        

        position = 0.0
        if time < offset_time:
            position = (initValue-offsetDistance + 1/3*acc*(offset_time-time)**3)
        time -= offset_time
        if 0<= time < accelerating_duration:
            position = initValue-offsetDistance + 1/3 * acc * time**3
        time -= accelerating_duration
        # End Phase
        if 0 <= time < accelerating_duration:
            position += initValue-offsetDistance +accelerating_distance*2 +  1/3 * acc * (time-accelerating_duration)**3
        time -= accelerating_duration

        # Final Phase
        if time >= 0.0:
            position += targetValue

        return position

# Define parameters
initVelocity = 0.0
initial_position = 100
target_position = -200.0
expected_velocity = 300.0  # Initial velocity at t=0
acceleration = 50.0

# Create time values
time_values = np.linspace(0, 10, 600)  # Adjust the time range as needed

# Calculate position values
position_values = [
    calculate_position(t, initial_position, target_position,initVelocity, expected_velocity, acceleration )
    for t in time_values
]

# Create time values
time_values = np.linspace(0, 10, 600)  # Adjust the time range as needed

# Calculate position and velocity values
position_values = [
    calculate_position(t, initial_position, target_position,initVelocity, expected_velocity, acceleration)
    for t in time_values
]

velocity_values = [
    calculate_speed(t, initial_position, target_position,initVelocity, expected_velocity, acceleration)
    for t in time_values
]

# Plotting Position vs Time
plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(time_values, position_values, label="Position vs Time")
plt.title("Position vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Position")
plt.legend()
plt.grid(True)

# Plotting Velocity vs Time
plt.subplot(1, 2, 2)
plt.plot(time_values, velocity_values, label="Velocity vs Time", color='orange')
plt.title("Velocity vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Velocity")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()