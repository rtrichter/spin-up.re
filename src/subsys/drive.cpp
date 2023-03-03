#include "drive.hpp"
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
        int left_error, right_error;
        int left_voltage, right_voltage;
        while(true)
        {
            left_error = vleft-sens::left.get_velocity();
            left_voltage = Kvt*vleft + Kpt*left_error;
            right_error = vright-sens::right.get_velocity();
            right_voltage = Kvt*vright + Kpt*right_error;
            set_tank(left_voltage, right_voltage);
        }
    }

    void linear_displacement_profile_control(int v0, int vf, int xf)
    {
        int x0, x = sens::avg_drive_encoder();
        int v = sens::avg_drive_encoder_velocity();
        float t = xf/(float(v0+vf)/2);
        int a = v0-vf/t;

        int v_profile, x_profile;

        int t0 = pros::millis();
        while(x<xf && v)
        {
            // time since start of the move
            float delta_t = float(pros::millis()-t0)/1000;
            // velocity the profile says we should be at
            int v_profile = a*delta_t;
            // current displacement from start
            x = sens::avg_drive_encoder()-x0;

        }
    }

    void translate(int distance, int velocity, float accel_period)
    {
        sens::tare_drive_encoders();
        int direction = distance / abs(distance);
        // distance input as centitiles
        // also makes it positive. direction handled later
        int SP = dist2enc(distance)*direction;
        int PV = sens::avg_drive_encoder();
        int error = SP-PV;
        // velocity input as rpm. convert to centidegrees/sec
        int vmax = rpm2enc(velocity);
        int accel = vmax/accel_period;
        int accel_disp = vmax*accel_period/2; // triangle displacement
        if (accel_disp>SP)
        {
            // these lines scale the triangle profile so the rate of accel
            // is the same as it would have been for the larger triangle
            // however the scale also ensures that the triangle has a 
            // displacement equal to SP
            float scale = sqrt((2*SP)/(accel_period*vmax));
            vmax *= scale;
            accel_period *= scale;
        }
        int v_actual = sens::avg_drive_encoder_velocity();
        int v_target;
        int v_profile, disp_profile;
        float delta_t, time_remaining;
        int I, Ti; // add an integral component to the velocity control system
        int t0 = pros::millis();
        int drive_diff, l_diff, r_diff;
        float drive_diff_Ti = 0.1;
        while (v_actual<vmax)
        {
            // time since the move started
            delta_t = (pros::millis()-t0)/1000; 
            // the velocity the bot SHOULD be at based on the motion profile
            v_profile = accel*delta_t;
            // the current displacement from the start point
            PV = sens::avg_drive_encoder();
            // the displacement the bot SHOULD be at based on the motion profile
            disp_profile = .5*accel*delta_t*delta_t;
            // The displacement remaining
            I = accel_disp-PV;
            // The time remaining
            Ti = accel_period-delta_t;
            // combine for PI control
            v_target = v_profile + I/Ti;
            // adjust left and right to make sure bot is 
            // traveling in a straight line
            // (integral control)
            drive_diff = sens::left.get_position()-sens::right.get_position();
            // decrease l if diff is positive (left is faster)
            // abs value not necessary for left but looks more consistent
            l_diff -= drive_diff/0.1*(drive_diff>0); 
            // same but += bc drive_diff will be negative if right is faster
            r_diff += drive_diff/0.1*(drive_diff<0);
            vleft = v_target + l_diff;
            vright = v_target + r_diff;
            pros::delay(10);
        }

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
        m::left.move_voltage(left);
        m::right.move_voltage(right);
    }

    // runs repeatedly during main's opcon function
   void opcon()
    {
        int l = get_mapped_input(ctrl::left);
        int r = get_mapped_input(ctrl::right);
        set_tank(l, r);
    }
}