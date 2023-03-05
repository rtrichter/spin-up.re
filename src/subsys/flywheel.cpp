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
        if (fabs(v1 - v2) > 5)
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
        float total_error=0;
        int voltage;
        while (true)
        {
            current = get_velo();
            error = velocity - current;
            total_error += error;
            voltage = Kv*velocity + Kp*error;
            // do not brake to get to 0
            if (!velocity)
                voltage = 0;
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
        if (abs(get_velo()-velocity)>5)
        {
            cout << "bad flywheel speed: " 
            << m::flywheel1.get_actual_velocity() - velocity << endl;
            return 0;
        }
        m::feed.move_relative(360, 200);
        pros::delay(50);
        return 1;
    }

    void tune_wait()
    {
        int t0 = pros::millis();
        int delta_v=100;
        int v, v_prev=0;
        do {
            if (pros::millis() - t0 > 5000) // timeout after 5 seconds
                return;
            v = get_velo();
            delta_v = v-v_prev;
            v_prev = v;
            pros::delay(25);
        } while (
            !(fabs(get_velo() - velocity) > 10 || fabs(delta_v) > 5)
        );
    }


    int tune()
    {
        cout << "tuning started" << endl;
        bool tuning = true;
        Kp = 0;
        int delta_kp = 100;
        int t0, t1, t2, t3, t4, delta_t;
        int prev_velo = 0;
        int min_delta_t = 10000000;
        int avg_delta_t;
        while (tuning)
        {
            avg_delta_t = 0;
            for (int i=0; i<3; i++)
            {
            t0 = pros::millis();
            // wait until at close speed
            velocity = velo_presets[0];
            tune_wait();
            t1 = pros::millis();
            pros::delay(200);
            // wait until stopped
            velocity = 0;
            tune_wait();
            pros::delay(200);
            t2 = pros::millis();
            // wait until at far speed
            velocity = velo_presets[1];
            tune_wait();
            t3 = pros::millis();
            pros::delay(200);
            velocity = 0;
            // wait until stopped
            pros::delay(200);
            t4 = pros::millis();


            delta_t = (t3-t2) + (t1-t0); // ignore time to stop
            avg_delta_t += delta_t;
            cout << 
            Kp << "," <<
            delta_t << ", ," <<
            t0 << "," <<
            t1-t0 << "," <<
            t2-t1 << "," <<
            t3-t2 << "," <<
            t4-t3 << "\n";
            }
            cout << Kp << "," << avg_delta_t << endl;

            if (avg_delta_t<min_delta_t)
            {
                min_delta_t = delta_t;
                Kp += delta_kp;
            }
            else
            {
                delta_kp /= 2;
                Kp -= delta_kp;
                if (delta_kp==1)
                    return Kp;
            }
        }
        return 0;
    }

    void telem_out()
    {
        cout <<
        "FLYWHEEL_TELEMETRY" << "," <<
        pros::millis() << "," <<
        velocity << "," <<
        get_velo() << endl;
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
        velocity = running*(velo_presets[!close] + v_tune);
        // check if a feed is being attempted
        if (ctrl::master.get_digital(ctrl::feed))
            feed();

        // tune the flywheel Kp value
        if (ctrl::master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
            tune();

        if (verbose)
            telem_out();
    }


}