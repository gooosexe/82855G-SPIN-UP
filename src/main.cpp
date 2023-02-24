#include "main.h"
#include <cmath>
#include <math.h>
#include <string>

pros::Controller master(pros::E_CONTROLLER_MASTER);

double inches = 200;
double rotation = 5.611; // rotation angle multiplier, multiply dist by this for any turn


// motors
pros::Motor mtr_lf(15);
pros::Motor mtr_lb(16);
pros::Motor mtr_rf(19);
pros::Motor mtr_rb(20);

pros::Motor rollerMtr(3);
//pros::Motor expansionMtr(1);

pros::Motor mtr_intake(9);
pros::Motor flywheel(10);
pros::Motor mtr_indexer(7);

// pneumatics+
pros::ADIDigitalOut pn_indexer ('C');
pros::ADIDigitalOut pn_expand ('A');
//pros::ADIDigital
//pros::ADIDigitalOb ut pn_expand2 ('B');

void moveLeftSide(int distance, int power) {
    mtr_lf.move_relative(-distance, power);
    mtr_lb.move_relative(-distance, power);
}

void moveRightSide(int distance, int power) {
    mtr_rf.move_relative(distance, power);
    mtr_rb.move_relative(distance, power);
}

void moveAll(int distance, int power) {
    mtr_lf.move_relative(-distance, power);
    mtr_lb.move_relative(-distance, power);
    mtr_rf.move_relative(distance, power);
    mtr_rb.move_relative(distance, power);
}

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
    pros::lcd::set_text(1, "Hello PROS User!");

    pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

double rotationAngle = 5.611;

void turn(double angle, int power) {
    moveLeftSide(-angle*rotation, power);
    moveRightSide(angle*rotation, power);
}

/* void turnRight90() {
    moveLeftSide(505, 80);
    moveRightSide(-505, 80);

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



void competition_initialize() {}


void autonomous() {

 // LS auto                                                                                                                               

    // roller 1
    moveAll(-300, 80);
    pros::delay(500);
    rollerMtr.move_relative(480, 127);
    pros::delay(600);
    moveAll(300, 80);
    pros::delay(800);

	// 2 low goals

    turn(-90, 80);
    pros::delay(1000);
	moveAll(370, 80);
    flywheel = 110;
    pros::delay(2000);
    mtr_indexer.move_relative(3500, 90);
    pros::delay(2000);

	// intake stack and shoot

    moveLeftSide(770, 80);
	moveRightSide(-770, 80);
    pros::delay(1500);
    mtr_intake = -127;
    moveAll(-2800 , 127);

    pros::delay(3000);

    moveLeftSide(590, 80);
	moveRightSide(-590, 80);                                                
                
    pros::delay(2000);                           

    flywheel = 127;

	moveAll(450, 80);

    pros::delay(3000);

    mtr_indexer.move_relative(3500, 110);

    pros::delay(3000);

	moveAll(-500, 80);

    flywheel = 0;


//RS auto
    // moveAll(-3000, 80);

    // pros::delay(100);

    // turnRight90();

    // pros::delay(100);

    // moveAll(-300, 80);
    // pros::delay(100);
    // rollerMtr.move_relative(480, 127);

    // pros::delay(100);
    // moveAll(1000, 80);

    // pros::delay(100);

    // moveRightSide(20, 80);

    // flywheel = 127;
    // pros::delay(1000);

    // mtr_indexer.move_relative(3500, 127);

    // pros::delay(500);

    // flywheel = 0;

    // turnLeft90();
    // moveLeftSide(200, 80);
    // pros::delay(500);

    // mtr_intake = -127;




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
