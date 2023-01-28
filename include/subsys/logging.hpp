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


#ifndef _LOGGING_H_
#define _LOGGING_H_

namespace m
{
    inline pros::Motor motors[8] = {
        left, right,
        flywheel1, flywheel2, feed,
        intake,
        roller,
    }

}

namespace logging
{
    inline const string path = "/usd/spin-up/logs/";
    inline const string prefix = "log_";
    inline const string extension = ".csv"; 
    inline ofstream log_file;
    bool file_exists(const std::string& name);
    string get_filename();
    void init();
    
}

#endif