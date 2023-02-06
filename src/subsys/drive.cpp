#include "drive.hpp"
#include "globals.hpp"
#include <cmath>

// define motors used for drive
pros::Motor m::left (p::left, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor m::right (p::right, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// define drive controls
pros::controller_analog_e_t ctrl::left = pros::E_CONTROLLER_ANALOG_LEFT_Y;
pros::controller_analog_e_t ctrl::right = pros::E_CONTROLLER_ANALOG_RIGHT_Y;

namespace drive
{
    // maps analog input to the first concave up section of a cosine wave (0 -> PI/4)
    int get_mapped_input(pros::controller_analog_e_t axis)
    {
        // get analog input
        int x = ctrl::master.get_analog(axis);
        // return 0 if the input is less than the deadzone threshold
        if (abs(x) < ctrl::deadzone_threshold)
            return 0;
        // math explained in readme
        int value = (2/(2-sqrt(2))) * (1- cos( double((abs(x)-ctrl::deadzone_threshold)*PI) / double((127-ctrl::deadzone_threshold)*4))) * Vmax;
        // set value to negative if input was negative
        // needed bc the equation uses absolute value
        if (x < 0) value*=-1;
        return value;
    }

    // set velocity of each side of the tank drive
    void set_tank(int left, int right)
    {
        m::left = left;
        m::right = right;
    }

    // runs repeatedly during main's opcon function
    void opcon()
    {
        int l = get_mapped_input(ctrl::left);
        int r = get_mapped_input(ctrl::right);
        set_tank(l, r);
    }
}