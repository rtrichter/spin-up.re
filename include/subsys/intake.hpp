#include "main.h"

#ifndef _INTAKE_H_
#define _INTAKE_H_

namespace p
{
    inline const int intake = 21;
}

namespace m
{
    extern pros::Motor intake;
}

namespace ctrl
{
    extern pros::controller_digital_e_t intake_toggle;
    extern pros::controller_digital_e_t intake_reverse;
}

namespace intake
{
    inline const int v = 600;
    inline bool running;
    void toggle();
    void run();
    void opcon();
}

#endif