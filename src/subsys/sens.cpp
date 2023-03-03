#include "sens.hpp"

namespace sens
{
    pros::Rotation left(p::left_r, true);
    pros::Rotation right(p::right_r, true);
    pros::Imu gyro(p::gyro);
    pros::Optical color(p::color);
    pros::Vision vis(p::vis);

    void tare_drive_encoders()
    {
        sens::left.set_position(0);
        sens::right.set_position(0);
    }

    // returns [0, 360)
    int get_direction()
    {
        return int(gyro.get_rotation()) % 360;
    }
}