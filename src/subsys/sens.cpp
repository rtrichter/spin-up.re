#include "sens.hpp"

namespace sens
{
    pros::Rotation left(p::left_r, true);
    pros::Rotation right(p::right_r, true);
    pros::Imu gyro(p::gyro);

    float avg_drive_encoder()
    {
        return (sens::left.get_position() + sens::right.get_position())/2.0;
    }

    float avg_drive_encoder_velocity()
    {
        return (sens::left.get_velocity() + sens::right.get_velocity())/2.0;
    }

    void tare_drive_encoders()
    {
        sens::left.set_position(0);
        sens::right.set_position(0);
    }

    // returns [0, 360)
    int get_direction()
    {
        return (int(gyro.get_rotation()) % 360)*bool(int(gyro.get_rotation())%360);
    }
    float get_accel_x()
    {
        return gyro.get_accel().x*1600;
    }
    bool drive_is_moving()
    {
        return (abs(left.get_velocity()) || abs(right.get_velocity()));
    }
}
