#include "subsys/roller.hpp"
#include "subsys/globals.hpp"

// define roller motors
pros::Motor m::roller (p::roller, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

// define roller controls
pros::controller_digital_e_t roller_down = pros::E_CONTROLLER_DIGITAL_B;
pros::controller_digital_e_t roller_up = pros::E_CONTROLLER_DIGITAL_X;

namespace roller
{
    void opcon()
    {
        // roll down
        if (ctrl::master.get_digital(ctrl::roller_down))
            m::roller = v;
        // roll up
        if (ctrl::master.get_digital(ctrl::roller_up))
            m::roller = v * -1;
    }

}