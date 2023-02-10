#include "main.h"

#ifndef _EXPANSION_H_
#define _EXPANSION_H_

namespace p
{
    inline const int expansion = 14;
}

namespace m
{
    extern pros::Motor expansion;
}

namespace ctrl
{
    extern pros::controller_digital_e_t exp1;
    extern pros::controller_digital_e_t exp2;
    extern pros::controller_digital_e_t exp3;
}

namespace expansion
{
    void opcon();
}

#endif