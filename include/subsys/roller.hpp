#include "main.h"

#ifndef _ROLLER_H_
#define _ROLLER_H_

namespace p
{
    inline const int roller = 7;
}

namespace m
{
    extern pros::Motor roller;
}

namespace ctrl
{
    extern pros::controller_digital_e_t roller_down;
    extern pros::controller_digital_e_t roller_up;
}

namespace roller
{
    inline const int v = 100;
    void opcon();
}

#endif