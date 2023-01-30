#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "main.h"
#include "subsys/drive.hpp"
#include "subsys/flywheel.hpp"
#include "subsys/globals.hpp"
#include "subsys/intake.hpp"
#include "subsys/roller.hpp"
#include "subsys/expansion.hpp"


#ifndef _LOGGING_H_
#define _LOGGING_H_

namespace m
{
    inline pros::Motor motors[8] = {
        left, right,
        flywheel1, flywheel2, feed,
        intake,
        roller,
        expansion
    };
    inline string names[8] = {
        "left", "right",
        "flywheel1", "flywheel2", "feed",
        "intake",
        "roller",
        "expansion"
    };
}

namespace ctrl
{
    inline pros::controller_analog_e_t analog[2] = {
        left, right
    };
    inline pros::controller_digital_e_t digital[11] = {
        fw_toggle, feed, fw_close, fw_far,
        intake_toggle, intake_reverse,
        roller_down, roller_up,
        exp1, exp2, exp3
    };
    inline string names[13] = {
        "left", "right",
        "fw_toggle", "feed", "fw_close", "fw_far",
        "intake_toggle", "intake_reverse",
        "roller_down", "roller_up",
        "exp1", "exp2", "exp3"
    };
}

namespace logging
{
    inline const string path = "/usd/spin-up/logs/";
    inline const string prefix = "log_";
    inline const string extension = ".csv"; 
    inline ofstream log_file;
    bool file_exists(const std::string& name);
    string get_filename();
    void motor_init();
    void motor_data();
    void ctrl_init();
    void ctrl_data();
    void bat_init();
    void bat_data();
    void sens_init();
    void sens_data();
    void init();
    void record();
    
}

#endif