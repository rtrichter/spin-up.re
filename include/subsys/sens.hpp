#include "main.h"

#ifndef _SENS_H_
#define _SENS_H_

namespace p
{
    inline const int gyro = 1;
}

namespace gyro
{
    extern pros::Imu gyro;
    int get_direction();
    float get_accel_x();
}

#endif