#include "main.h"

#ifndef _AUTON_H_
#define _AUTON_H_

/*

https://en.wikipedia.org/wiki/PID_controller

P: Proportional
I: Integral
D: Derivative 
(think 0th, -1st, and 1st derivatives)

P:
SP = setpoint (target value)
PV = process variable (measured value)
P = SP-PV (difference between actual and expected)

Integral:
Accounts for the past values of SP-PV error

Derivative:
Predictive control

OUR APPLICATION:

Proportional:
PV = rotation sensor value (converted)
SP = target value
e = SP-PV

Integral:
makes the input increase over time until the target is reached

Derivative:
Decreases control at end of move


Kp
Ki
Kd

error
prevError
derivative
integral 


PV = avgEncoderValue
error = SP-PV

derivative = error - prevError

integral += error (exclude from drive?)

voltage = Kp*error + Ki*integral + Kd*derivative





prevError = error
pros::delay(20)



*/

namespace flywheel
{
    void PID(int velocity);
}

namespace drive
{
    inline const double Kpt = 0.0;
    inline const double Kit = 0.0;
    inline const double Kdt = 0.0;
    inline const double Kpr = 0.0;
    inline const double Kir = 0.0;
    inline const double Kdr = 0.0;
    void translate(int distance);
    void rotate(int degrees);
}

#endif