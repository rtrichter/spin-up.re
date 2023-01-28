#include "main.h"

namespace p
{
    inline const int roller = 12;
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
    inline const int v = 200;
    void opcon();
}