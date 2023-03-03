#include "main.h"
#include "pros/rotation.hpp"

#ifndef _SENS_H_
#define _SENS_H_

namespace p
{
    inline const int gyro = 1;
    inline const int left_r = 19;
    inline const int right_r = 20;
    inline const int vis = 22;
    inline const int color = 22;
}


namespace sens
{
    extern pros::Rotation left;
    extern pros::Rotation right;
    extern pros::Imu gyro;
    extern pros::Vision vis;
    extern pros::Optical color;
    float avg_drive_encoder();
    float avg_drive_encoder_velocity();
    void tare_drive_encoders();
    int get_direction();
}

#endif