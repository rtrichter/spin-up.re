#include "main.h"

#ifndef _AUTON_H_
#define _AUTON_H_


namespace drive
{
    /*
    COORDINATE SYSTEM
    x is horizontal from drivers pov
    y is vertical from drivers pov
    the driver's low goal is 0,0
    one tile = 100 centitiles (ct) 
    rotations:
    +y = 0
    -y = 180
    +x = 90
    -x = 270
    */
    inline int x;
    inline int y;

    void translate(int distance, int velocity);
    void rotate(int degrees, int velocity);
    void rotate_to(int degrees, int velocity);
    void move_relative(int x, int y, int rotation);
    void moveto(int x, int y, int rotation);
}


#endif