#include "logging.hpp"
#include "pros/motors.h"

#include "drive.hpp"
#include "flywheel.hpp"
#include "globals.hpp"
#include "intake.hpp"
#include "roller.hpp"
#include "expansion.hpp"

namespace p
{
    const int motors[8] = 
    {
        left, right,
        flywheel1, flywheel2, feed, 
        intake,
        roller,
        expansion
    };
}

namespace m
{
    string names[8] = {
        "left", "right",
        "flywheel1", "flywheel2", "feed",
        "intake",
        "roller",
        "expansion"
    };
}

namespace ctrl
{
    pros::controller_analog_e_t analog[2] = {
        left, right
    };
    pros::controller_digital_e_t digital[11] = {
        fw_toggle, feed, fw_close, fw_far,
        intake_toggle, intake_reverse,
        roller_down, roller_up,
        exp1, exp2, exp3
    };
    string names[13] = {
        "left", "right",
        "fw_toggle", "feed", "fw_close", "fw_far",
        "intake_toggle", "intake_reverse",
        "roller_down", "roller_up",
        "exp1", "exp2", "exp3"
    };
}

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
            m::names[i] << " voltage";
        }
    }

    void bat_init()
    {
        // write column headers for the battery
        log_file << "," <<
        "battery capacity," <<
        "battery current," <<
        "battery temperature," <<
        "battery voltage";
    }

    void ctrl_init()
    {
        // write column headers for the battery
        log_file << "," <<
        "ctrl battery capacity," <<
        "ctrl battery level," <<
        "ctrl connected"; 
        for (int i=0; i<13; i++)
        {
            log_file << "," << ctrl::names[i];
        }
    }

    void ctrl_data()
    {
        // write column headers for ctrl
        log_file << "," <<
        ctrl::master.get_battery_capacity() << "," <<
        ctrl::master.get_battery_level() << "," <<
        ctrl::master.is_connected();
        for (int i=0; i<2; i++)
        {
            log_file << "," <<
            ctrl::master.get_analog(ctrl::analog[i]);
        }
        for (int i=0; i<11; i++)
        {
            log_file << "," <<
            ctrl::master.get_digital(ctrl::digital[i]);
        }
    }

    void bat_data()
    {
        // write data for the battery
        log_file << "," <<
        pros::battery::get_capacity() << "," <<
        pros::battery::get_current() << "," <<
        pros::battery::get_temperature() << "," <<
        pros::battery::get_voltage();
    }


    void motor_data()
    {
        // write data for the motors
        for (int i=0; i<8; i++)
        {
            log_file << "," <<
            pros::c::motor_get_target_velocity(p::motors[i]) << "," <<
            pros::c::motor_get_actual_velocity(p::motors[i]) << "," <<
            pros::c::motor_get_temperature(p::motors[i]) << "," <<
            pros::c::motor_get_efficiency(p::motors[i]) << "," <<
            pros::c::motor_get_position(p::motors[i]) << "," <<
            pros::c::motor_get_power(p::motors[i]) << "," <<
            pros::c::motor_get_torque(p::motors[i]) << "," <<
            pros::c::motor_get_voltage(p::motors[i]);
        }
    }



    void init()
    {
        fname = get_filename();
        log_file.open(fname, fstream::out);
        log_file << pros::millis(); // 1
        motor_init(); // 8**8
        ctrl_init(); // 17
        bat_init(); // 4
        log_file << "\n";
        log_file.close();
    }

    void record()
    {
        log_file.open(fname, fstream::app);
        log_file << pros::millis();
        motor_data();
        ctrl_data();
        bat_data();
        log_file << "\n";

        log_file.close();
    }
}