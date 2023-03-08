#include "roller.hpp"
#include "globals.hpp"
#include "sens.hpp"

// define roller motors
pros::Motor m::roller (p::roller, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// define roller controls
pros::controller_digital_e_t ctrl::roller_down = pros::E_CONTROLLER_DIGITAL_B;
pros::controller_digital_e_t ctrl::roller_up = pros::E_CONTROLLER_DIGITAL_X;
pros::controller_digital_e_t ctrl::roller_auto = pros::E_CONTROLLER_DIGITAL_A;

namespace roller
{
    void auto_roll()
    {
        m::roller.move_velocity(100);
        int stuck_time=0;
        while (abs(sens::color.get_hue() - my_color) > 10)
        {
            pros::delay(10);
            if (m::roller.get_actual_velocity()<20)
                stuck_time += 1;
            if (stuck_time >= 20)
                break;
        }
        m::roller.move_velocity(0);
        if (!(abs(sens::color.get_hue() - my_color) > 10))
            return;
        m::roller.move_velocity(-100);
        pros::delay(100);
        while (abs(sens::color.get_hue() - my_color) > 10)
        {
            pros::delay(10);
            if (m::roller.get_actual_velocity()<20)
                stuck_time += 1;
            if (stuck_time >= 20)
                break;
        }
        m::roller.move_velocity(0);
    }

    void opcon()
    {
        // roll down
        if (ctrl::master.get_digital(ctrl::roller_down))
            m::roller = v;
        // roll up
        else if (ctrl::master.get_digital(ctrl::roller_up))
            m::roller = v * -1;
        else if (ctrl::master.get_digital(ctrl::roller_auto))
            auto_roll();
        else
            m::roller = 0;
        
    }

}