#include "sens.hpp"

namespace gyro
{
    pros::Imu gyro(p::gyro);
    // returns [0, 360)
    int get_direction()
    {
        return (360 - int(gyro.get_rotation()) % 360)*bool(int(gyro.get_rotation())%360);
    }
    float get_accel_x()
    {
        return gyro.get_accel().x*1600;
    }
}