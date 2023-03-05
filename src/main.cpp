#include "main.h"
#include <cmath>
#include <math.h>
#include <string>

pros::Controller master(pros::E_CONTROLLER_MASTER);

double inches = 200;

// motors
pros::Motor mtr_lf(15);
pros::Motor mtr_lb(16);
pros::Motor mtr_rf(19);
pros::Motor mtr_rb(20);

pros::Motor rollerMtr(3);

pros::Motor mtr_intake(9);
pros::Motor flywheel(10);
pros::Motor mtr_indexer(7);

// pneumatics+
pros::ADIDigitalOut pn_expand ('A');

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

void turnRight90() {
    moveLeftSide(510, 80);
    moveRightSide(-510, 80);

}

void turnLeft90() {
    moveLeftSide(-510, 80);
    moveRightSide(510, 80);
}

void flipRight() {
    moveLeftSide(1010, 80);
    moveRightSide(-1010, 80);
}

void flipLeft() {
    moveLeftSide(-1010, 80);
    moveRightSide(1010, 80);
}

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "PUSHBOT META");

    pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void leftAuto(){

        // roller 1
        moveAll(-300, 80);
        pros::delay(500);
        rollerMtr.move_relative(480, 127);
        pros::delay(600);
        moveAll(200, 80);
        pros::delay(800);

        // 2 low goals
        turnRight90();
        pros::delay(1000);
        moveAll(550, 80);
        flywheel = 110;
        pros::delay(2000);
        mtr_indexer.move_relative(3500, 127);
        pros::delay(2000);

        //intake stack and shoot
        moveLeftSide(725, 80);
        moveRightSide(-725, 80);
        pros::delay(1500);
        mtr_intake = -127;
        moveAll(-3000 , 127);
        pros::delay(3000);
        moveLeftSide(585, 80);
        moveRightSide(-585, 80);                                                 
        pros::delay(1700);                          
        flywheel = 127;
        pros::delay(1500);
        moveAll(100, 127);
        mtr_indexer.move_relative(3500, 127);

}

void rightAuto(){

        // low goals
        flywheel = 115;
        pros::delay(3000);
        mtr_indexer.move_relative(3000, 127);
        pros::delay(3500);

        // roller
        flywheel = 0;
        moveAll(-inches*11, 100);
        pros::delay(100);
        pros::delay(1000);
        moveLeftSide(570, 50); // counterclock wise
        moveRightSide(-570, 50);
        pros::delay(1000);
        moveAll(-inches*2, 80);
        pros::delay(1000);
        rollerMtr.move_relative(470, 127);
}

void skillsAuto(){

    //move to roller
    moveAll(-inches*12, 100);
    pros::delay(1000);
    moveLeftSide(520, 50); // clockwise
    moveRightSide(-520, 50);
    pros::delay(800);
    moveAll(-inches*2, 80);
    pros::delay(1000);

    // do roller
    rollerMtr.move_relative(850, 127);
    pros::delay(1000);

    // shoot low goals
    moveAll(inches*3, 80); // move back
    pros::delay(1000);
    moveLeftSide(-670, 80); // face goals, counter clockwise, greater value = more towards outside
    moveRightSide(670, 80);
    pros::delay(500);
    moveAll(200, 80);
    pros::delay(200);
    flywheel = 115;
    pros::delay(2000);
    mtr_indexer.move_relative(3000, 127);
    pros::delay(2000);

    // intake 3 discs
    moveLeftSide(-840, 80); // turn (greater value, towards alliance low goal)
    moveRightSide(840, 80);
    pros::delay(1000);
    mtr_intake = -127;
    moveAll(-3800, 127);
    pros::delay(4000);

    // shoot high goals
    moveLeftSide(-585, 80);
    moveRightSide(585, 80);
    pros::delay(1500);
    moveAll(100, 127);
    mtr_indexer.move_relative(3500, 90);
    pros::delay(3000);

    // intake stack
    moveAll(-100, 127);
    moveLeftSide(585, 80);
    moveRightSide(-585, 80);
    pros::delay(3000);
    moveAll(-2000, 127);
    pros::delay(3000);

    //shoot high goal 2nd time
    moveLeftSide(-520, 80); // face high goal, counter clockwise
    moveRightSide(520, 80);  
    pros::delay(1000);
    moveAll(200, 127);
    mtr_indexer.move_relative(3500, 90);
    pros::delay(3000);

    // do left side roller
    moveLeftSide(750, 80); // turn clockwise
    moveRightSide(-750, 80);
    moveAll(-1000, 127);
    pros::delay(1000);
    moveLeftSide(-250, 80); // turn counter clockwise
    moveRightSide(250, 80);
    pros::delay(250);
    moveAll(-300, 80);
    pros::delay(300);
    rollerMtr.move_relative(850, 127);
    pros::delay(1000);

    // expand
    moveAll(500, 80);
    pros::delay(500);
    moveLeftSide(250, 80); // turn clockwise
    moveRightSide(-250, 80);
    pros::delay(300);
    pn_expand.set_value(1);

}

void autonomous() {

    // leftAuto();
    // rightAuto();
    skillsAuto();

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
        if (master.get_digital(DIGITAL_R1)) {
            rollerMtr = -70;
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
            mtr_indexer = 70;
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

