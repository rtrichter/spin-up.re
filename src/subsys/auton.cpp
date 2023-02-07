#include "auton.hpp"

#include "drive.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "globals.hpp"
#include "intake.hpp"
#include "roller.hpp"
#include "sens.hpp"

#include <cmath>

namespace drive
{

    // degrees to tiles (used for centidegrees to centitiles)
    float d2t(int degrees)
    {
        // degrees *
        // (1 rotation / 360 degrees) *
        // (4PI inches / 1 rotation) *
        // (1 tile / 24 inches) *
        // (2/1) (gear ration)
        // simplifies to centidegrees*PI/1080
        return degrees*PI/1080;
    }
    // tiles to degrees (used for centitiles to centidegrees)
    float t2d(int tiles)
    {
        // tiles *
        // (24 inches / 1 tile)
        // (1 rotation / 4PI inches)
        // (360 degrees / 1 rotation)
        // (1/2) (gear ration)
        // simplifies to ct*2160/PI
        return tiles*1080/PI;
    }

    void translate(int distance, int velocity) 
    {
        int direction = abs(distance)/distance;
        sens::tare_drive_encoders();
        set_tank(velocity*direction, velocity*direction);
        // move until you get to target position
        while (d2t(sens::avg_drive_encoder())*direction < (distance-30))
            pros::delay(10);
        // brake
        set_tank(-velocity*direction, -velocity*direction);
        while (sens::avg_drive_encoder_velocity()*direction > 0)
            pros::delay(10);
        // turn off drive motors when the bot comes to rest
        set_tank(0,0);
        // correct overshoot
        int error = d2t(sens::avg_drive_encoder())*direction - distance;
        if (abs(error) > 30)
            // equation: (x degrees/0.1 s)(1rotation/360degrees)(60s/min)()
            translate(error, abs(t2d(error*100)/6));
    }

    void rotate(int degrees, int velocity)
    {
        int initial_rotation = sens::gyro.get_rotation();
        int direction = abs(degrees) / degrees;
        set_tank(velocity * direction, -velocity * direction);
        while (fabs(sens::gyro.get_rotation()-initial_rotation) < (abs(degrees)-30))
            pros::delay(10);
        // brief brake
        set_tank(-velocity*direction, velocity*direction);
        // wait to come to rest
        int r0;
        int r=sens::gyro.get_rotation(); // set to r0 in do/while
        do {
            r0=r;
            pros::delay(10);
            r = sens::gyro.get_rotation();
        } while (r != r0); // loop until not rotating
        set_tank(0, 0);
        // if degrees and gyro rotation are not the same
        int error = (sens::gyro.get_rotation()-initial_rotation) - degrees;
        if (error > 2)
            rotate(error, abs(error)*35/6);

        // // if (abs(degrees - int(sens::gyro.get_rotation()-initial_rotation)))
        // //     // rotate the amount required to fix rotation at half speed
        // //     // recursive until within 1 degree of target according to gyro
        // //     rotate(degrees - sens::gyro.get_rotation()-initial_rotation, velocity/2);



        // // if overshot
        // if (abs(int(sens::gyro.get_rotation()-initial_rotation)) > abs(degrees))
        // {
        //     set_tank(-50 * direction, 50 * direction);
        //     while (fabs(sens::gyro.get_rotation()-initial_rotation) > abs(degrees))
        //         {cout << "fixing overshoot" << endl;
        //         pros::delay(10);}
        //     set_tank(velocity*direction, -velocity*direction);
        // }
        // // if undershot
        // else if (fabs(sens::gyro.get_rotation()-initial_rotation) < abs(degrees))
        // {
        //     set_tank(50 * direction, -50 * direction);
        //     while (fabs(sens::gyro.get_rotation()) < abs(degrees))
        //     {    cout << "fixing undershoot" << endl;
        //         pros::delay(10);}
        //     set_tank(-velocity*direction, velocity*direction);
        // }
        // set_tank(0,0);
    }

    void rotate_to(int degrees, int velocity)
    {
        int delta_theta = degrees - sens::get_direction();
        if (delta_theta <= 180)
            rotate(delta_theta, velocity);
        else
            rotate(delta_theta-360, velocity);
    }

    void move_relative(int x, int y, int rotation, int velocity)
    {
        // find angle to move at
        int theta;
        if (!y) // protect against div0 errors
        {
            theta = 90;
        }
        else 
        {
            theta = atan(float(x)/y)*180/PI;
        }
        int quadrant = (x<0) + (y<0)*2;
        switch (quadrant)
        {
            // quadrant 1 has no change
            case 1: // quadrant 2
                theta = 360-theta;
                break;
            case 2: // quadrant 4
                theta = 180-theta;
                break;
            case 3: // quadrant 3
                theta = 180+theta;
                break;
        }

        // set direction to that angle
        rotate_to(theta, velocity);
        
        // find distance to translate
        int dist = sqrt(x*x+y*y);
        // translate that distance
        translate(dist, velocity);
        // rotate to desired rotation
        rotate_to(rotation, velocity);
    }
}