#include "main.h"

#ifndef _AUTON_H_
#define _AUTON_H_

namespace auton
{
    void set_auton_select_screen();
    void on_btn0();
    void on_btn1();
    void on_btn2();
    inline int routine=0;
    inline bool confirmed = false;
    void none();
    void left_high();
    void left_low();
    void left_wp();
    void right_high();
    void right_low();
    void right_wp();
    void skills();
    inline const int max_routine=7;
    inline const string names[] = {
        "none", 
        "left_high", "left_low", "left_wp", 
        "right_high", "right_low", "right_wp",
        "skills"
    };
    typedef void (*void_func) ();
    inline const void_func routines[] = {
        none, 
        left_high, left_low, left_wp,
        right_high, right_low, right_wp,
        skills
    };
    void auton();
}


#endif