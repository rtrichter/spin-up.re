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
    inline const int Vmax = 12000;
    inline const float wheel_size = 4;
    inline const float wheelbase = 14;
    int get_mapped_input(pros::controller_analog_e_t axis);
    inline int vleft=0;
    inline int vright=0;
    void velocity_control(void* param);
    void set_tank(int left, int right);

    float in2rot(float distance);
    float deg2rot(float degrees);

    void move_tank(float distance, int velocity);
    void rotate_tank(float degrees, int velocity);
    
    void opcon();

}


#endif