#include "main.h"

#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsys/drive.hpp"
#include "subsys/flywheel.hpp"
#include "subsys/intake.hpp"
#include "subsys/roller.hpp"
#include "subsys/expansion.hpp"
#include "subsys/logging.hpp"
#include "subsys/sens.hpp"
#include "subsys/globals.hpp"
#include "subsys/auton.hpp"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::Task flywheel_speed(flywheel::velocity_control);
	pros::Task drive_speed(drive::velocity_control);
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb(auton::on_btn0);
	pros::lcd::register_btn1_cb(auton::on_btn1);
	pros::lcd::register_btn2_cb(auton::on_btn2);

	// initialize logging system
	logging::init();
	pros::Task log_task(logging::log_task);
	// reset sensors
	sens::gyro.reset();
	sens::left.reset();
	sens::right.reset();
	// set all motor brake types
	m::left.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::right.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::feed.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::flywheel1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::flywheel2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::roller.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	m::expansion.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	auton::auton();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	
	int count = 0;
	while (true)
	{
		drive::opcon();
		flywheel::opcon();
		intake::opcon();
		roller::opcon();
		expansion::opcon();
		if (!(count%5))
		{
			cout << count << endl;
			logging::record();
		}
		pros::delay(10);
		count++;
	}
}
