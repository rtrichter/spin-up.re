#include "main.h"
#include "pros/rotation.hpp"

#ifndef _SENS_H_
#define _SENS_H_

namespace p
{
    inline const int gyro = 1;
    inline const int left_r = 19;
    inline const int right_r = 20;
}


namespace sens
{
    extern pros::Rotation left;
    extern pros::Rotation right;
    extern pros::Imu gyro;
    float avg_drive_encoder();
    float avg_drive_encoder_velocity();
    void tare_drive_encoders();
    int get_direction();
    float get_accel_x();
    bool drive_is_moving();
}

#endif