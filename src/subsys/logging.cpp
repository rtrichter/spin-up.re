#include "subsys/logging.hpp"

namespace logging
{
    // checks if a file exists
    bool file_exists(const std::string& name)
    {
        ifstream f(name.c_str());
        return f.good();
    }

    // finds next filename to create
    string get_filename()
    {
        // max number of recordings before things break
        int possibleMax = 10000;

        // check every file with "path/log_#.csv" format until one is not found
        for (int i=0; i<possibleMax; i++)
        {
            string fname = "";
            fname.append(path);
            fname.append(prefix);
            fname.append(to_string(i));
            fname.append(extension);
            // when a file is found that doesn't exist, return that filename
            if (!file_exists(fname))
                return fname;
        }
        // if the loop fails to find the next file to create, use log_0.csv
        return path + prefix + "0" + extension;
    }

    void motor_init()
    {
        // write column headers for the motors
        for (int i=0; i<8; i++)
        {
            log_file << "," <<
            m::names[i] << " target_velocity," <<
            m::names[i] << " actual_velocity," <<
            m::names[i] << " temperature," <<
            m::names[i] << " efficiency," <<
            m::names[i] << " position," <<
            m::names[i] << " power," <<
            m::names[i] << " torque," <<
            m::names[i] << " voltage,";
        }
    }

    void motor_data()
    {
        // write data for the motors
        for (int i=0; i<8; i++)
        {
            log_file << "," <<
            m::motors[i].get_target_velocity() << "," <<
            m::motors[i].get_actual_velocity() << "," <<
            m::motors[i].get_temperature() << "," <<
            m::motors[i].get_efficiency() << "," <<
            m::motors[i].get_position() << "," <<
            m::motors[i].get_power() << "," <<
            m::motors[i].get_torque() << "," <<
            m::motors[i].get_voltage();
        }
    }

    void init()
    {
        log_file.open(get_filename());
        log_file << pros::millis();
        motor_init();
        log_file << "\n";
    }

    void record()
    {
        log_file << pros::millis();
        motor_data();
        log_file << "\n";
    }
}