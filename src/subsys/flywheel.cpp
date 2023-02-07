#include "flywheel.hpp"
#include "globals.hpp"

// define flywheel and feed motors
pros::Motor m::flywheel1 (p::flywheel1, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor m::flywheel2 (p::flywheel2, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor m::feed (p::feed, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

// define flywheel and feed controls
pros::controller_digital_e_t ctrl::fw_toggle = pros::E_CONTROLLER_DIGITAL_R1;
pros::controller_digital_e_t ctrl::feed = pros::E_CONTROLLER_DIGITAL_R2;
pros::controller_digital_e_t ctrl::fw_far = pros::E_CONTROLLER_DIGITAL_UP;
pros::controller_digital_e_t ctrl::fw_close = pros::E_CONTROLLER_DIGITAL_DOWN;

namespace flywheel
{
    // toggle running on or off
    void toggle()
    {
        running = !running;
    }

    // set the appropriate speed depending on close or far
    void set_speed()
    {
        if (close)
            speed = Vclose;
        else
            speed = Vfar;
    }

    // spin the flywheel at a given velocity
    void spin(int velocity)
    {
        m::flywheel1.move_velocity(velocity * running);
        m::flywheel2 .move_velocity(velocity * running);
    }

    // feed a disk into the flywheel
    // aka shoot
    void feed()
    {
        // do not try to feed a disc until feed pusher is done spinning
        if (m::feed.get_actual_velocity()) 
            return;
        // do not shoot unless flywheel is at the correct speed
        if (m::flywheel1.get_actual_velocity() < speed)
            return;
        m::feed.move_relative(363, 200);
    }

    void opcon()
    {
        // check if running should be toggled
        if (ctrl::master.get_digital_new_press(ctrl::fw_toggle))
            toggle();
        // check if flywheel speed should be set to far
        if (ctrl::master.get_digital_new_press(ctrl::fw_far))
            close = false;
        // check if flywheel speed should be set to close
        if (ctrl::master.get_digital_new_press(ctrl::fw_close))
            close = true;
        // update the flywheel speed
        set_speed();
        spin(speed*running);
        if (ctrl::master.get_digital(ctrl::feed))
            feed();


    }


}