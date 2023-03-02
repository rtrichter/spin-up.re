#include "auton.hpp"

using namespace pros::lcd;

namespace auton
{

// auton select
void set_auton_select_screen()
{
    clear();
    if (confirmed)
    {
        set_text(
            0, 
            "Confirmed and prepared to run " + names[routine]
        );
    }
    set_text(
        0, 
        "Currently selecting " + names[routine]
    );
}

void on_btn0()
{
    // confirm locks selection process
    if (confirmed)
        return;
    // wrap to max value if decreased at 0
    if (!routine)
    {
        routine = max_routine;
        return; 
    }
    routine -= 1;
    set_auton_select_screen();
}

void on_btn1()
{
    // toggle whether or not the auton is confirmed
    confirmed = !confirmed;
    set_auton_select_screen();
}

void on_btn2()
{
    // confirm locks selection process
    if (confirmed)
        return;
    // wrap to min value if increased at max
    if (routine == max_routine)
    {
        routine = 0;
        return;
    }
    routine += 1;
    set_auton_select_screen();
}


// routines
void none()
{
    cout << "running \"none\" auton" << endl;
}

void left_high()
{
    cout << "running \"left_high\" auton" << endl;
}

void left_low()
{
    cout << "running \"left_low\" auton" << endl;
}

void left_wp()
{
    cout << "running \"left_wp\" auton" << endl;
}

void right_high()
{
    cout << "running \"right_high\" auton" << endl;
}

void right_low()
{
    cout << "running \"right_low\" auton" << endl;
}

void right_wp()
{
    cout << "running \"right_wp\" auton" << endl;
}

void skills()
{
    cout << "running \"skills\" auton" << endl;
}

void auton()
{
    clear();
    routines[routine]();
}

}