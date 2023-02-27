#include "main.h"
#include <cmath>
#include <math.h>
#include <string>
#include "autonFunctions.h"
#include "motorDef.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// pneumatics+
// pros::ADIDigitalOut pn_indexer ('C');
pros::ADIDigitalOut pn_expand ('A');
//pros::ADIDigital
//pros::ADIDigitalOb ut pn_expand2 ('B');

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "PUSHBOT META");

    pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

void autonomous() {
    drivePID();
}

void competition_initialize() {}

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

double strafeAngle; // stores the angle the left stick is pointing

void opcontrol() {
    while (true) {
        pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                         (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                         (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

        double ymotion = master.get_analog(ANALOG_LEFT_Y);
        double xmotion = master.get_analog(ANALOG_LEFT_X);
        double rotation = master.get_analog(ANALOG_RIGHT_X);
        
        // if roller pressed
        if (master.get_digital(DIGITAL_R1)) {
            rollerMtr = -115;
        } else {
            rollerMtr = 0;
        }
        
        //flywheel
        if (master.get_digital(DIGITAL_L2)) {
                flywheel = -127;
        } else {
                flywheel = 127;
        }

        // if expand pressed
        if (master.get_digital(DIGITAL_B)) {
            pn_expand.set_value(1);
        } else {
            pn_expand.set_value(0);
        }
        //indexer
        if (master.get_digital(DIGITAL_R2)) {
            mtr_indexer = 90;
        } else {
            mtr_indexer = 0;
        }

        // intake
        if (master.get_digital(DIGITAL_L1)) {
            mtr_intake = 127;
        } else {
            mtr_intake = -127;
        }

        //drive
        //strafeAngle = atan2(ymotion, xmotion)*180/M_PI; // find angle (only used in the second snipped)

        int LF = -ymotion - rotation;
        int RF = ymotion - rotation;
        int LB = -ymotion - rotation;
        int RB = ymotion - rotation;

        mtr_lf = LF;
        mtr_rf = RF;
        mtr_lb = LB;
        mtr_rb = RB;
        
        pros::lcd::set_text(0, "Left stick X: " + std::to_string(xmotion));
        pros::lcd::set_text(1, "Left stick Y: " + std::to_string(ymotion));
        pros::lcd::set_text(2, "LF power : " + std::to_string(mtr_lf.get_power()) + "\t RB power: "+ std::to_string(mtr_rb.get_power()));
        pros::lcd::set_text(3, "LB power: " + std::to_string(mtr_lb.get_power()) + "\t RF power: "+ std::to_string(mtr_rf.get_power()));
        pros::lcd::set_text(4, "Strafe angle: " + std::to_string(strafeAngle));
        pros::lcd::set_text(5, "Flywheel speed: " + std::to_string(flywheel.get_actual_velocity()));

        pros::delay(1);
    }
}

