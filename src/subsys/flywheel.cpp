#include "subsys/flywheel.hpp"
#include "subsys/globals.hpp"

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

    float get_velo()
    {
        float v1 = m::flywheel1.get_actual_velocity();
        float v2 = m::flywheel2.get_actual_velocity();
        // protect against variations in readings
        // removes (most) outliers by making sure both readings are near
        // each other
        if (fabs(v1 - v2) > 7)
            return get_velo();
        return (v1 + v2) / 2;
    }
    
    void velocity_control(void* param)
    {
        float current;
        float error = 1;
        float prev_error = 0;
        float integral;
        float derivative;
        int voltage;
        while (true)
        {
            current = get_velo();
            error = velo_presets[preset] - current;
            delta_v = error; // for tuning
            integral += error;
            derivative = error-prev_error;
            voltage = volt_presets[preset] + Kp*error + Ki*integral + Kd*derivative;
            prev_error = error;
            pros::delay(10);
        }
    }

    // feed a disk into the flywheel
    int feed()
    {
        // do not try to feed a disc until feed pusher is done spinning
        if (m::feed.get_actual_velocity()) 
        {
            cout << "still feeding" << endl;
            return 0;
        }
        // do not shoot unless flywheel is at the correct speed
        if (abs(m::flywheel1.get_actual_velocity() - speed)>10)
        {
            cout << "bad flywheel speed: " << m::flywheel1.get_actual_velocity() - speed << endl;
            return 0;
        }
        m::feed.move_relative(360, 200);
        pros::delay(50);
        return 1;
    }

    void tune_wait()
    {
        int t0 = pros::millis();
        while (get_velo() && !delta_v)
        {
            
            pros::delay(10);
            if (pros::millis() - t0 > 10000) // timeout after 10 seconds
                break;
        }
    }


    int tune()
    {
        bool tuning = true;
        int delta_kp = 1000;
        int t0, t1, t2, t3, t4, delta_t;
        int prev_velo = 0;
        int min_delta_t = 10000;
        while (tuning)
        {
            t0 = pros::millis();
            // wait until at close speed
            preset = 1;
            tune_wait();
            t1 = pros::millis();
            // wait until stopped
            preset = 0;
            tune_wait();
            t2 = pros::millis();
            // wait until at far speed
            preset = 2;
            tune_wait();
            t3 = pros::millis();
            preset = 0;
            // wait until stopped
            t4 = pros::millis();


            delta_t = t4-t0;
            cout << 
            Kp << "," <<
            delta_t << ", ," <<
            t0 << "," <<
            t1 << "," <<
            t2 << "," <<
            t3 << "," <<
            t4 << "\n";

            if (delta_t<min_delta_t)
            {
                min_delta_t = delta_t;
                Kp += delta_kp;
            }
            else
            {
                Kp -= delta_kp;
                delta_kp /= 2;
            }
            if (delta_kp < 5)
                return Kp;
        }
    }


    void opcon()
    {
        // check if running should be toggled
        if (ctrl::master.get_digital_new_press(ctrl::fw_toggle))
            toggle();
        // check if flywheel speed should be set to far
        if (ctrl::master.get_digital_new_press(ctrl::fw_far))
            preset = 1;
        // check if flywheel speed should be set to close
        if (ctrl::master.get_digital_new_press(ctrl::fw_close))
            preset = 2;
        if (ctrl::master.get_digital(ctrl::feed))
            feed();

        if (ctrl::master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
            tune();
    }


}