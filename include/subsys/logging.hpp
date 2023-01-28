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