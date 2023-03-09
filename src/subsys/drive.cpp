#include "drive.hpp"
#include "pros/misc.h"
#include "sens.hpp"
#include "globals.hpp"
#include <cmath>

// define motors used for drive
pros::Motor m::left 
    (p::left, 
    pros::E_MOTOR_GEARSET_18, 
    false, 
    pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor m::right 
    (p::right, 
    pros::E_MOTOR_GEARSET_18, 
    true, 
    pros::E_MOTOR_ENCODER_DEGREES);

// define drive controls
pros::controller_analog_e_t ctrl::left = pros::E_CONTROLLER_ANALOG_LEFT_Y;
pros::controller_analog_e_t ctrl::right = pros::E_CONTROLLER_ANALOG_RIGHT_Y;

namespace drive
{
    int dist2enc(int distance)
    {
        return distance*(24./1)*(360./wheel_size*PI);
    }

    int rmp2enc(int rpm)
    {   // converts from rpm to centidegrees per second
        return rpm*(3600./1)*(1./60);
    }


    void velocity_control(void* param)
    {
        // int left_error, right_error;
        // int left_voltage, right_voltage;
        while(true)
        {
            // left_error = vleft-sens::left.get_velocity();
            // left_voltage = Kvt*vleft + Kpt*left_error;
            // right_error = vright-sens::right.get_velocity();
            // right_voltage = Kvt*vright + Kpt*right_error;
            set_tank(vleft, vright);
        }
    }

    int ct2deg(int x)
    {
        return x*(24*360)/(100*4*PI);
    }

    void wait_to_stop()
    {
        while (sens::drive_is_moving())
        {
            pros::delay(10);
        }     
    }
    
    void translate(int distance, int v)
    {
        int d = ct2deg(distance);
        cout << d << endl;
        m::left.move_relative(d,v);
        m::right.move_relative(d,v);
        pros::delay(200);
    }

    void rotate(int degrees, int velocity)
    {
        int d = ct2deg(wheelbase*PI*(degrees/360.)*(100./24));
        cout << d << endl;
        m::left.move_relative(d, velocity);
        m::right.move_relative(-d, velocity);
        pros::delay(200);
    }



    // maps analog input to the first concave up section of a cosine wave 
    // (0 -> PI/4)
    int get_mapped_input(pros::controller_analog_e_t axis)
    {
        // get analog input
        int x = ctrl::master.get_analog(axis);
        int sign = x/abs(x);
        // return 0 if the input is less than the deadzone threshold
        if (abs(x) < ctrl::deadzone_threshold)
            return 0;
        int value = (2/(2-sqrt(2))) * 
        (1- cos( double((abs(x)-ctrl::deadzone_threshold)*PI) / 
        double((127-ctrl::deadzone_threshold)*4))) * velocity_cap * sign;
        // // set value to negative if input was negative
        // // needed bc the equation uses absolute value
        // if (x < 0) value*=-1;
        return value;
    }

    // set velocity of each side of the tank drive
    void set_tank(int left, int right)
    {
        m::left.move_velocity(left);
        m::right.move_velocity(right);
    }

    void test()
    {
        cout << "testing\n" << endl;
        translate(100, 150);
        cout << "finished translate\n";
        wait_to_stop();
        cout << "finished wait\n";
        pros::delay(100);
        rotate(180, 150);
        wait_to_stop();
        cout << "finished rotate\n" << endl;
    }

    // runs repeatedly during main's opcon function
   void opcon()
    {
        int l = get_mapped_input(ctrl::left);
        int r = get_mapped_input(ctrl::right);
        vleft = l;
        vright = r;
        set_tank(l,r);
        // if (ctrl::master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
        // {
        //     cout << "about to test\n";
        //     test();
        // }
    }
}