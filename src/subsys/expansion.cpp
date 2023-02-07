#include "expansion.hpp"
#include "globals.hpp"

// define motors
pros::Motor m::expansion (p::expansion, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

// define controls
pros::controller_digital_e_t ctrl::exp1 = pros::E_CONTROLLER_DIGITAL_A;
pros::controller_digital_e_t ctrl::exp2 = pros::E_CONTROLLER_DIGITAL_LEFT;
pros::controller_digital_e_t ctrl::exp3 = pros::E_CONTROLLER_DIGITAL_R2;

namespace expansion
{
    void shoot()
    {
        m::expansion.move_relative(360, 200);
    }

    void opcon()
    {
        if (ctrl::master.get_digital(ctrl::exp1)
            && ctrl::master.get_digital(ctrl::exp2)
            && ctrl::master.get_digital_new_press(ctrl::exp3))
            shoot();
    }
}