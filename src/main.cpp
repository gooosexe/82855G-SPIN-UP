#include "main.h"
#include <cmath>
#include <math.h>
#include <string>
#include "autonFunctions.h"
#include "globals.h"
#include "pros/llemu.hpp"

using namespace pros;

// pneumatics+
// ADIDigitalOut pn_indexer ('C');

//ADIDigital
//ADIDigitalOb ut pn_expand2 ('B');


void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        lcd::set_text(2, "I was pressed!");
    } else {
        lcd::clear_line(2);
    }
}

void initialize() {
    lcd::initialize();
    lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

void autonomous() {
    leftAuton();
}

void competition_initialize() {}

double velocity;

void opcontrol() {
    while (true) {
        lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                         (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                         (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

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
            mtr_indexer = 50;
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

        int LF = ymotion + rotation;
        int RF = ymotion - rotation;
        int LB = ymotion + rotation;
        int RB = ymotion - rotation;

        mtr_lf = LF;
        mtr_rf = RF;
        mtr_lb = LB;
        mtr_rb = RB;
        
        lcd::set_text(1, "Right P, I, D: " + std::to_string(rightpE * propGain) + " " + std::to_string(rightiE * integGain) + " " + std::to_string(rightdE * derivGain)); 
        lcd::set_text(2, "Left P, I, D: " + std::to_string(leftpE * propGain) + " " + std::to_string(leftiE * integGain) + " " + std::to_string(leftdE * derivGain));
        delay(1);
    }
}