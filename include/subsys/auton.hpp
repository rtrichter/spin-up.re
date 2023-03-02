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


namespace drive
{
    /*
    COORDINATE SYSTEM
    x is horizontal from drivers pov
    y is vertical from drivers pov
    the driver's low goal is 0,0
    one tile = 100 centitiles (ct) 
    rotations:
    +y = 0
    -y = 180
    +x = 90
    -x = 270
    */
    inline int x;
    inline int y;

    void translate(int distance, int velocity);
    void rotate(int degrees, int velocity);
    // void rotate_to(int degrees, int velocity);
    void move_relative(int x, int y, int rotation);
    void moveto(int x, int y, int rotation);
}

namespace flywheel
{
    int repeat_fire(int repeats, int timeout=10000);
}

namespace roller
{
    int auto_roll_s(int direction, int timeout=10000);
    int auto_roll(int direction, int timeout=10000);
}

namespace routines
{
    void left_low();
    void left_high();
    void right_low();
    void right_high();
    void skills();
}

#endif