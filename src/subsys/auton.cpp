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

    // degrees to tiles (used for degrees to tiles)
    float d2t(int degrees)
    {
        // degrees *
        // (1 rotation / 360 degrees) *
        // (4PI inches / 1 rotation) *
        // (1 tile / 24 inches) *
        // (2/1) (gear ration)
        // simplifies to degrees*PI/1080
        return degrees*PI/1080;
    }
    // tiles to degrees (used for tiles to degrees)
    float t2d(int tiles)
    {
        // tiles *
        // (24 inches / 1 tile)
        // (1 rotation / 4PI inches)
        // (360 degrees / 1 rotation)
        // (1/2) (gear ration)
        // simplifies to tiles*1080/PI
        return tiles*1080/PI;
    }

    void translate(int distance, int velocity) 
    {
        int direction = abs(distance)/distance;
        sens::tare_drive_encoders();
        cout << d2t(sens::avg_drive_encoder()*direction) << endl;
        set_tank(velocity*direction, velocity*direction);
        // move until you get to target position
        // while (d2t(sens::avg_drive_encoder())*direction < (distance-10))
        // *direction to make sure it is positive
        // could use abs but direction used for consistency with velocity stopping check
        while (d2t(sens::avg_drive_encoder()*direction)<(distance*direction-10))
        {
            pros::delay(10);
        }
        // brake
        set_tank(-4*direction, -4*direction);
        // wait until the drivetrain stops spinning
        // *direction makes velocity go from positive to negative
        while (sens::avg_drive_encoder_velocity()*direction > 0) 
            pros::delay(10);
        // turn off drive motors when the bot comes to rest
        set_tank(0,0);
        // correct overshoot
        int error = d2t(sens::avg_drive_encoder()) - distance;
        if (error>1)
        {
            // equation: (x degrees/0.1 s)(1rotation/360degrees)(60s/min)()
            translate(error, abs(t2d(error*100)/6));
        }
    }

    void rotate(int degrees, int velocity)
    {
        int delta_theta = degrees - sens::get_direction();
        int direction;

        if (delta_theta > 0)
        {
            direction = 1;
            set_tank(velocity, -velocity);
            do {
                pros::delay(10);
            }while (sens::get_direction()<degrees);
        }
        else
        {
            set_tank(-velocity, velocity);
            do {
                pros::delay(10);
            }while (sens::get_direction()>degrees);
        }
        // brief brake
        set_tank(-20*direction, 20*direction);
        // wait to come to rest
        int r0;
        int r=sens::gyro.get_rotation(); // set to r0 in do/while
        do {
            r0=r;
            pros::delay(10);
            r = sens::gyro.get_rotation();
        } while (r != r0); // loop until not rotating
        // stop drive when the bot comes to rest
        set_tank(0, 0);
        // correct any error
        int error = abs((sens::gyro.get_rotation())- degrees);
        if (error)
            rotate(degrees, error*35/6);
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
            theta = atan(fabs(float(x)/y))*180/PI;
        }
        int quadrant = (x<0) + (y<0)*2;
        /*
        0 = q1
        1 = q2
        2 = q4
        3 = q3 
        */
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

namespace flywheel
{
    int repeat_fire(int repeats, int timeout)
    {
        int start_ts = pros::millis();
        for (int i=0; i<repeats; i=i)
        {
            // returns 1 on success which breaks out of the loop after 2 shots
            i += flywheel::feed();
            pros::delay(10);
            if ((pros::millis() - start_ts)>timeout)
                return 0;
        }
        return 1;
    }
}

namespace roller
{
    int auto_roll(int direction, int timeout)
    {
        int start_ts = pros::millis();
        drive::set_tank(-100, -100);
        pros::delay(200);
        while (sens::avg_drive_encoder_velocity())
        {
            pros::delay(10);
            if (pros::millis()-start_ts > timeout)
                return 0;
        }
        // // spin roller some amount
        m::roller.move_relative(70*direction, 100);
        drive::set_tank(0,0);
        return 1;
    }
}

namespace routines
{
    void right_low()
    {
        // start flywheel (SLOW)
        flywheel::spin(250);
        // // shoot two discs 
        // for (int i=0; i<2; i=i)
        // {
        //     // returns 1 on success which breaks out of the loop after 2 shots
        //     i += flywheel::feed();
        //     pros::delay(10);
        // }
        flywheel::repeat_fire(2);
        flywheel::spin(0);
        // move back 1 tile ish
        drive::translate(-70, 100);
        // // turn 90 degrees
        drive::rotate_to(90, 100);
        // // push against roller
        roller::auto_roll(-1);
    }

    void left_low()
    {
        // start flywheel
        flywheel::spin(250);
        // spin roller
        roller::auto_roll(-1);
        // move away
        drive::translate(20, 100);
        // turn
        drive::rotate_to(-90, 100);
        // shoot twice
        flywheel::repeat_fire(2);
        // stop flywheel
        flywheel::spin(0);
    }

    void skills()
    {
        // shoot 2 high goals
        // start flywheel
        // flywheel::spin(flywheel::Vclose);
        // // aim for high goal
        // flywheel::repeat_fire(2);
        // flywheel::spin(0);




        // back up 
        drive::translate(-230, 150);
        // turn to first roller
        drive::rotate_to(90, 150);
        // roll
        roller::auto_roll(1);
        // move out
        drive::translate(100, 150);
        // turn to 2nd roller
        drive::rotate_to(0, 150);
        // turn roller
        roller::auto_roll(1);
        // move out
        drive::translate(100, 150);
        // turn towards opposite corner
        drive::rotate_to(50, 150);
        // drive a few miles forward
        drive::translate(500, 150);
        // turn for first roller
        // push against roller
        // turn roller
        // move out
        // turn to last roller
        // push against roller
        // spin roller
        // move away
        // turn for expansion
        // fire expansion
    }
}