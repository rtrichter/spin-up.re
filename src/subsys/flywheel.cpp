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
pros::controller_digital_e_t ctrl::fw_tune_up = pros::E_CONTROLLER_DIGITAL_RIGHT;
pros::controller_digital_e_t ctrl::fw_tune_down = pros::E_CONTROLLER_DIGITAL_LEFT;

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
        cout << "velo control started" << endl;;
        // feed forward and feedback control
        float current;
        float error = 1;
        float prev_error = 0;
        float integral;
        float derivative;
        int voltage;
        while (true)
        {
            // cout << "running velo control | ";
            current = get_velo();
            // cout << "current velo = " << current << "\n";
            // cout << "target velo = " << velo_presets[preset] << "\n";
            error = velocity - current;
            // cout << "error = " << error << " | ";
            delta_v = error; // for tuning
            // cout << "delta_v = " << delta_v << " | ";
            integral += error;
            derivative = error-prev_error;
            voltage = Kv*velocity + Kp*error + Ki*integral + Kd*derivative;
            // cout << "voltage = " << voltage << "\n";
            prev_error = error;
            m::flywheel1.move_voltage(voltage);
            m::flywheel2.move_voltage(voltage);
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
        if (abs(m::flywheel1.get_actual_velocity() - velocity)>10)
        {
            cout << "bad flywheel speed: " << m::flywheel1.get_actual_velocity() - velocity << endl;
            return 0;
        }
        m::feed.move_relative(360, 200);
        pros::delay(50);
        return 1;
    }

    void tune_wait()
    {
        int t0 = pros::millis();
        do {
            // cout << get_velo() << " | " << velo_presets[preset] << " | " << delta_v << endl;
            pros::delay(25);
            if (pros::millis() - t0 > 5000) // timeout after 5 seconds
                return;
        } while (fabs(get_velo() - velocity) > 10 && fabs(delta_v) > 5);
    }


    int tune()
    {
        cout << "tuning started" << endl;
        bool tuning = true;
        int delta_kp = 1000;
        int t0, t1, t2, t3, t4, delta_t;
        int prev_velo = 0;
        int min_delta_t = 10000000;
        int avg_delta_t;
        while (tuning)
        {
            avg_delta_t = 0;
            for (int i=0; i<5; i++)
            {
            t0 = pros::millis();
            // wait until at close speed
            velocity = vclose;
            tune_wait();
            // pros::delay(500);
            t1 = pros::millis();
            // wait until stopped
            velocity = 0;
            tune_wait();
            // pros::delay(500);
            t2 = pros::millis();
            // wait until at far speed
            velocity = vfar;
            tune_wait();
            // pros::delay(500);
            t3 = pros::millis();
            velocity = 0;
            // wait until stopped
            // pros::delay(500);
            t4 = pros::millis();


            delta_t = t4-t0;
            avg_delta_t += delta_t;
            cout << 
            Kp << "," <<
            delta_t << ", ," <<
            t0 << "," <<
            t1 << "," <<
            t2 << "," <<
            t3 << "," <<
            t4 << "\n";
            }
            cout << Kp << "," << avg_delta_t << endl;

// 0,21160, ,982,10992,12132,22142,22142
// 1000,3040, ,22142,22472,22862,25182,25182
// 2000,3050, ,25182,25542,25932,28232,28232
// 1000,2720, ,28232,28262,28712,30952,30952
// 1500,4350, ,30952,32582,32972,35302,35302
// 1000,3340, ,35302,35872,36262,38642,38642
// 750,3540, ,38642,39312,39692,42182,42182
// 625,3780, ,42182,43062,43462,45962,45962
// 563,3680, ,45962,46762,47142,49642,49642
// 532,3760, ,49642,50422,50902,53402,53402
// 517,3680, ,53402,54052,54492,57082,57082


            if (avg_delta_t<min_delta_t)
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
        return 0;
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
        // check if tuning should increase
        if (ctrl::master.get_digital_new_press(ctrl::fw_tune_up))
            v_tune += v_tune_delta;
        // check if tuning should decrease
        if (ctrl::master.get_digital_new_press(ctrl::fw_tune_down))
            v_tune -= v_tune_delta;
        // set velocity based on distance and tuning
        velocity = velo_presets[running*(1+!close)] + v_tune;
        // check if a feed is being attempted
        if (ctrl::master.get_digital(ctrl::feed))
            feed();

        // tune the flywheel Kp value
        if (ctrl::master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
            tune();
    }


}