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
    void translate(int distance, int velocity) 
    {
        int direction = abs(distance) / distance;
        int x=0;
        int v=0;
        int v0=0;
        set_tank(velocity*direction, velocity*direction);
        int t0 = pros::millis();
        int t;
        while (abs(x) < abs(distance))
        {
            t = pros::millis(); 
            v0 =v;
            v += gyro::get_accel_x()*(t-t0);
            x += (v+v0)*(t-t0)/2;
            pros::delay(10);
        }
    }
    void rotate(int degrees, int velocity)
    {
        int initial_rotation = gyro::gyro.get_rotation();
        int direction = abs(degrees) / degrees;
        set_tank(velocity * direction, -velocity * direction);
        while (fabs(gyro::gyro.get_rotation()-initial_rotation) < abs(degrees))
            pros::delay(10);
        // brief brake
        set_tank(-velocity*direction, velocity*direction);
        // wait to come to rest
        int r0;
        int r=gyro::gyro.get_rotation(); // set to r0 in do/while
        do {
            r0=r;
            pros::delay(10);
            r = gyro::gyro.get_rotation();
        } while (r != r0); // loop until not rotating
        set_tank(0, 0);
        // if degrees and gyro rotation are not the same
        if (abs(degrees - int(gyro::gyro.get_rotation()-initial_rotation)))
            // rotate the amount required to fix rotation at half speed
            // recursive until within 1 degree of target according to gyro
            rotate(degrees - gyro::gyro.get_rotation()-initial_rotation, velocity/2);
        // // if overshot
        // if (abs(int(gyro::gyro.get_rotation()-initial_rotation)) > abs(degrees))
        // {
        //     set_tank(0.5 * -velocity * direction, 0.5 * velocity * direction);
        //     while (fabs(gyro::gyro.get_rotation()-initial_rotation) > abs(degrees))
        //         pros::delay(10);
        //     set_tank(velocity*direction, -velocity*direction);
        // }
        // // if undershot
        // else if (fabs(gyro::gyro.get_rotation()-initial_rotation) < abs(degrees))
        // {
        //     set_tank(0.5 * velocity * direction, 0.5 * -velocity * direction);
        //     while (fabs(gyro::gyro.get_rotation()) < abs(degrees))
        //         pros::delay(10);
        //     set_tank(-velocity*direction, velocity*direction);
        // }
        // set_tank(0,0);

    }

    void rotate_to(int degrees, int velocity)
    {
        int delta_theta = degrees - gyro::get_direction();
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