#include "auton.hpp"

#include "drive.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "globals.hpp"
#include "intake.hpp"
#include "roller.hpp"
#include "sens.hpp"

#include <cmath>

namespace flywheel
{

}

namespace drive
{
    int enc2dist(int encoder_value)
    {
        /*
        enc_value
        (4in*PI/enc_value degrees) inches
        (100 centitiles/24 inches) 
        */
    }

    /*

    AUTOTUNING

    try translate at varying Kp values
    

    
    */

    void translate(int distance)
    {
        // convert distance to encoder units
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
            current = sens::avg_drive_encoder();
            current = (sens::left.get_position() + sens::right.get_position()) / 2;
            error = distance - current;
            integral += error;
            derivative = error - prev_error;
            voltage = Kpt*error + Kit*integral + Kdt*derivative; // should be -12000 to 12000
            set_tank(voltage, voltage);
            pros::delay(10);
        }
        set_tank(0, 0);
    }
}