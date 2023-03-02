#include "intake.hpp"
#include "globals.hpp"

// define intake motor
pros::Motor m::intake(p::intake, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

// define intake controls
pros::controller_digital_e_t ctrl::intake_toggle = pros::E_CONTROLLER_DIGITAL_L1;
pros::controller_digital_e_t ctrl::intake_reverse = pros::E_CONTROLLER_DIGITAL_L2;

namespace intake
{
    // toggle running
    void toggle() { running = !running; }
    // run intake at velocity v if running is true
    void run(int speed) { m::intake = speed; }

    // runs during opcon
    void opcon()
    {
        // if reverse button is being pressed, run reverse
        if (ctrl::master.get_digital(ctrl::intake_reverse))
        {
            run(v*-1);
            // exit opcon function before motor is reset by run()
            return;
        }
        // check intake_toggle, toggle if necessary
        if (ctrl::master.get_digital_new_press(ctrl::intake_toggle))
            toggle();
        // set intake speed
        run(v * running);
    }
}