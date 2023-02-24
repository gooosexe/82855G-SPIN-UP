#include "main.h"
#include <cmath>
#include <math.h>
#include <string>

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

// gear ratio 3:7

// Motors
Motor mtr_lf(15, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_ROTATIONS);
Motor mtr_lb(16, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_ROTATIONS);
Motor mtr_rf(19, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_ROTATIONS);
Motor mtr_rb(20, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_ROTATIONS);

Motor rollerMtr(3);
Motor mtr_intake(9);
Motor flywheel(10);
Motor mtr_indexer(7);

// pneumatics+
ADIDigitalOut pn_indexer ('C');
ADIDigitalOut pn_expand ('A');

// gear ratio 3:7
// (360(distance/(4.125*M_PI))*(3/7)) allows input for inches

// move functions

void moveLeftSide(int distance, int power) {
    mtr_lf.move_relative((distance / (4.125*M_PI) * (3/7)), power);
    mtr_lb.move_relative((distance / (4.125*M_PI) * (3/7)), power);
}

void moveRightSide(int distance, int power) {
    mtr_rf.move_relative((distance / (4.125*M_PI)), power);
    mtr_rb.move_relative((distance / (4.125*M_PI)), power);
}

void moveAll(int distance, int power) {
    mtr_lf.move_relative((distance / (4.125*M_PI)) * (3/7), power); // (360(distance/(4.125*M_PI))*(3/7)) 
    mtr_lb.move_relative((distance / (4.125*M_PI)) * (3/7), power); // allows input for inches
    mtr_rf.move_relative((distance / (4.125*M_PI)) * (3/7), power);
    mtr_rb.move_relative((distance / (4.125*M_PI)) * (3/7), power);
}

// we need to change this lol (the distances)
void turnRight90() {
    moveLeftSide(510, 80);
    moveRightSide(-510, 80);

}

void turnLeft90() {
    moveLeftSide(-505, 80);
    moveRightSide(505, 80);
}

void flipRight() {
    moveLeftSide(1010, 80);
    moveRightSide(-1010, 80);
}

void flipLeft() {
    moveLeftSide(-1010, 80);
    moveRightSide(1010, 80);
} */

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
    lcd::set_text(1, "Hello PROS User!");

    lcd::register_btn1_cb(on_center_button);
}

void disabled() {}



void competition_initialize() {}


void autonomous() {

    turnRight90();
    delay(2000);
    turnLeft90();
    delay(2000);
    flipRight();
    delay(2000);
    flipLeft();

// LS auto                                                                                                                               

    // moveAll(-300, 80);

    // delay(500);

    pros::delay(3000);

    // delay(600);

    flywheel = 127;

    // delay(500);

    // moveLeftSide(525, 80);
    // moveRightSide(-525, 80);
    
    // delay(500);

    mtr_indexer.move_relative(3500, 110);

    pros::delay(3000);

    // delay(2000);

    flywheel = 0;

    // delay(2000);

//RS auto
    // moveAll(-3000, 80);

    // pros::delay(100);

    // delay(1500);

    // pros::delay(100);

    // moveAll(-300, 80);
    // pros::delay(100);
    // rollerMtr.move_relative(480, 127);

    // pros::delay(100);
    // moveAll(1000, 80);

    // delay(3000);

    // moveLeftSide(700, 80);git pu
    // moveRightSide(-700, 80);                                                        
                
    // delay(2000);                           

    // flywheel = 127;

    // delay(8000);

    // mtr_indexer.move_relative(3500, 127);

    // delay(3000);

    // flywheel = 0;

//RS auto
    // mtr_lf.move_relative(inches*10, 100); // Move forwards
    // mtr_lb.move_relative(inches*10, 100);
    // mtr_rf.move_relative(-inches*10, 100);
    // mtr_rb.move_relative(-inches*10, 100);
    // delay(100);
    // mtr_lf.move_relative(inches*10, 100); // Move forwards
    // mtr_lb.move_relative(inches*10, 100);
    // mtr_rf.move_relative(-inches*10, 100);
    // mtr_rb.move_relative(-inches*10, 100);
    // delay(100);
    // mtr_lf.move_relative(inches*10, 100); // Move forwards
    // mtr_lb.move_relative(inches*10, 100);
    // mtr_rf.move_relative(-inches*10, 100);
    // mtr_rb.move_relative(-inches*10, 100);

    // delay(1000);

    // mtr_lf.move_relative(-570, 50); // Turn clockwise
    // mtr_lb.move_relative(-570, 50);
    // mtr_rf.move_relative(-570, 50);
    // mtr_rb.move_relative(-570, 50);

    // delay(1000);

    // mtr_lf.move_relative(inches*2, 80); // Move forwards
    // mtr_lb.move_relative(inches*2, 80);
    // mtr_rf.move_relative(-inches*2, 80);
    // mtr_rb.move_relative(-inches*2, 80);

    // delay(1000);

    // rollerMtr.move_relative(470, 127);

    // delay(1000);

    // mtr_lf.move_relative(-inches*3, 80); // Move back
    // mtr_lb.move_relative(-inches*3, 80);
    // mtr_rf.move_relative(inches*3, 80);
    // mtr_rb.move_relative(inches*3, 80);

    // delay(1000);

    // mtr_intake = -127;

    // delay(200);


    // delay(3000);

}

//double strafeAngle; // stores the angle the left stick is pointing

void opcontrol() {
    while (true) {
        lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                         (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                         (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

        double ymotion = master.get_analog(ANALOG_LEFT_Y);
        double xmotion = master.get_analog(ANALOG_LEFT_X);
        double rotation = master.get_analog(ANALOG_RIGHT_X);
        
        // if roller pressed
        if (master.get_digital(DIGITAL_L1)) {
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
            mtr_indexer = 127;
        } else {
            mtr_indexer = 0;
        }

        // intake
        if (master.get_digital(DIGITAL_R1)) {
            mtr_intake = 127;
        } else {
            mtr_intake = -127;
        }

        // drive
        mtr_lf = ymotion - rotation;
        mtr_rf = ymotion - rotation;
        mtr_lb = ymotion - rotation;
        mtr_rb = ymotion - rotation;
        
        lcd::set_text(0, "Left stick X: " + std::to_string(xmotion));
        lcd::set_text(1, "Left stick Y: " + std::to_string(ymotion));
        lcd::set_text(2, "LF power : " + std::to_string(mtr_lf.get_power()) + "\t RB power: "+ std::to_string(mtr_rb.get_power()));
        lcd::set_text(3, "LB power: " + std::to_string(mtr_lb.get_power()) + "\t RF power: "+ std::to_string(mtr_rf.get_power()));
        //lcd::set_text(4, "Strafe angle: " + std::to_string(strafeAngle));
        lcd::set_text(5, "Flywheel speed: " + std::to_string(flywheel.get_actual_velocity()));

        delay(1);
    }
}
