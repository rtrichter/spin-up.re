#include "main.h"

#ifndef _FLYWHEEL_H_
#define _FLYWHEEL_H_

namespace p
{
    inline const int flywheel1 = 2;
    inline const int flywheel2 = 5;
    inline const int feed = 10;
}

namespace m
{
    extern pros::Motor flywheel1;
    extern pros::Motor flywheel2;
    extern pros::Motor feed;
}

namespace ctrl
{
    extern pros::controller_digital_e_t fw_toggle;
    extern pros::controller_digital_e_t feed;
    extern pros::controller_digital_e_t fw_far;
    extern pros::controller_digital_e_t fw_close;
}

namespace flywheel
{
    inline const int Vclose = 375;
    inline const int Vfar = 450;
    inline bool close = true;
    inline int speed;
    inline bool running = false;
    void toggle();
    void set_speed();
    void spin(int velocity);
    int feed();
    void opcon();

}

#endif