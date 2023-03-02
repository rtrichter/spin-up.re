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
    extern pros::controller_digital_e_t fw_tune_up;
    extern pros::controller_digital_e_t fw_tune_down;
}

namespace flywheel
{
    inline int velocity = 0;
    inline const float Kv=0.060473;
    inline int Kp=517;
    inline const int Ki=0;
    inline const int Kd=0;
    // used in tuning function
    inline int delta_v = 0;
    // 0: stopped
    // 1: close
    // 2: far
    inline const int velo_presets[] = {0, 335, 400};
    inline const int v_tune_delta = 5;
    inline int v_tune = 0;
    inline const int vclose = 335;
    inline const int vfar = 400;
    inline bool close = true;
    inline bool running = false;
    void toggle();
    void velocity_control(void* param);
    int feed();
    void opcon();

}

#endif