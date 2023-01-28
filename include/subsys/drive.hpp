#include "main.h"

#ifndef _DRIVE_H_
#define _DRIVE_H_

namespace p // ports
{
    inline const int left = 11;
    inline const int right = 13;
}

namespace m // motors
{
    extern pros::Motor left;
    extern pros::Motor right;
}

namespace ctrl
{
    extern pros::controller_analog_e_t left;
    extern pros::controller_analog_e_t right;

}

namespace drive
{
    inline const int Vmax = 200;
    inline const float wheel_size = 3.75;
    inline const float wheelbase = 16;
    int get_mapped_input(pros::controller_analog_e_t axis);
    void set_tank(int velocity);

    float in2rot(float distance);
    float deg2rot(float degrees);

    void move_tank(float distance, int velocity);
    void rotate_tank(float degrees, int velocity);
    
    void opcon();

}


#endif