// #include "auton.hpp"

// #include "drive.hpp"
// #include "expansion.hpp"
// #include "flywheel.hpp"
// #include "globals.hpp"
// #include "intake.hpp"
// #include "roller.hpp"
// #include "sens.hpp"

// #include <cmath>


namespace drive
{
    int enc2dist(int encoder_value)
    {
        /*
        enc_value
        (4in*PI/enc_value centidegrees) inches
        (1 tiles/24 inches) 
        */
        return encoder_value *
            (wheel_size*PI/360.) *
            (1/24.);
    }

    void rotate(int degrees)
    {
        sens::tare_drive_encoders();
        int distance = degrees * wheelbase*PI / 360. * 100/24;
        int current;
        int voltage;
        int error = 1;
        int prev_error = 0;
        int derivative;
        int integral;
        // while not at target angle and moving
        while ( error && sens::left.get_velocity() - sens::right.get_velocity()) 
        {
            current = enc2dist((
                    sens::left.get_position() - sens::right.get_position()
                    ) / 2);
            error = distance - current;
            integral += error;
            derivative = error - prev_error;
            voltage = Kpt*error + Kit*integral + Kdt*derivative; 
            set_tank(voltage, voltage);
            pros::delay(10);
        }
        set_tank(0, 0);
    }

    void translate(int distance)
    {
        sens::tare_drive_encoders();
        int current;
        int voltage;
        int error;
        int prev_error = 0;
        int derivative;
        int integral;
        // while not at target location and moving
        while ( error && sens::avg_drive_encoder_velocity() ) 
        {
            current = enc2dist((
                    sens::left.get_position() + sens::right.get_position()
                    ) / 2);
            error = distance - current;
            integral += error;
            derivative = error - prev_error;
            voltage = Kpt*error + Kit*integral + Kdt*derivative; 
            set_tank(voltage, voltage);
            pros::delay(10);
        }
        set_tank(0, 0);
    }
}
