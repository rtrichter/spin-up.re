#include <string>
#include <fstream>
using namespace std;

#include "main.h"


#ifndef _LOGGING_H_
#define _LOGGING_H_


namespace logging
{
    inline const string path = "/usd/spin-up/logs/";
    inline const string prefix = "log_";
    inline const string extension = ".csv"; 
    inline fstream log_file;
    inline string fname;
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
    void log_task(void* param);
    
}

#endif