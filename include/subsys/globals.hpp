#include "main.h"

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define PI 3.141593

inline int my_color;

namespace ctrl
{
    extern pros::Controller master;
    inline const int deadzone_threshold = 5;
}


#endif