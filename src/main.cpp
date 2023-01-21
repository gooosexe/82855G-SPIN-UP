#include "main.h"
#include <cmath>
#include <math.h>
#include <string>

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


void competition_initialize() {}


void autonomous() {

	

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
pros::Controller master(pros::E_CONTROLLER_MASTER);

// motors
pros::Motor mtr_lf(17);
pros::Motor mtr_lb(9);
pros::Motor mtr_rf(16);
pros::Motor mtr_rb(20);

pros::Motor rollerMtr(15);
pros::Motor expansionMtr(1);

pros::Motor mtr_intake(100);

// pneumatics
pros::ADIDigitalOut pn_indexer ('C');
pros::ADIDigitalOut pn_expand1 ('A');
pros::ADIDigitalOut pn_expand2 ('B');

double strafeAngle; // stores the angle the left stick is pointing

void opcontrol() {
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		double ymotion = master.get_analog(ANALOG_LEFT_Y);
		double xmotion = master.get_analog(ANALOG_LEFT_X);
		double rotation = master.get_analog(ANALOG_RIGHT_X);
		//double roller = master.get_digital(DIGITAL_R1);
		//double expansion = master.get_digital(DIGITAL_L1);
		
		// if roller pressed
		if (master.get_digital(DIGITAL_L1)) {
			rollerMtr = -80;
		} else {
			rollerMtr = 0;
		}

		// if expand pressed
		if (master.get_digital(DIGITAL_A)) {
			pn_expand1.set_value(1);
			pn_expand2.set_value(1);
		} else {
			pn_expand1.set_value(0);
			pn_expand2.set_value(0);
		}
		
		// indexer
		if (master.get_digital(DIGITAL_R2)) {
			pn_indexer.set_value(1);
			pros::delay(100);
			pn_indexer.set_value(0);
		}

		// intake
		if (master.get_digital(DIGITAL_R1)) {
			mtr_intake = 127;
		} else {
			mtr_intake = 0;
		}

		//drive
		strafeAngle = atan2(ymotion, xmotion)*180/M_PI; // find angle (only used in the second snipped)

		// calculating rotation values and strafe values for each motor (i might have to change the rotation later)
		int LF = (ymotion - xmotion + rotation);
		int RF = -(ymotion + xmotion - rotation);
		int LB = (ymotion + xmotion + rotation);
		int RB = -(ymotion - xmotion - rotation);

    	// normalize the powers of the motors so that no value exceeds 127
		int maxPower = abs(LF);
		if (abs(RF) > maxPower) maxPower = abs(RF);
		if (abs(LB) > maxPower) maxPower = abs(LB);
		if (abs(RB) > maxPower) maxPower = abs(RB);
		if (maxPower > 127) {
			LF = LF * 127 / maxPower;
			RF = RF * 127 / maxPower;
			LB = LB * 127 / maxPower;
			RB = RB * 127 / maxPower;
		}

		mtr_lf = LF;
		mtr_rf = RF;
		mtr_lb = LB;
		mtr_rb = RB;

		/* 	this also works as a replacement for the code above
		   	however the code above is what chatGPT generated so im going to keep it
		mtr_lf = -(cos((strafeAngle - 45)*(M_PI/180)) * sqrt(pow(ymotion, 2) + pow(xmotion, 2)));
		mtr_rb = cos((strafeAngle - 45)*(M_PI/180)) * sqrt(pow(ymotion, 2) + pow(xmotion, 2));

		mtr_rf = sin((strafeAngle - 45)*(M_PI/180)) * sqrt(pow(ymotion, 2) + pow(xmotion, 2));
		mtr_lb = -(sin((strafeAngle - 45)*(M_PI/180)) * sqrt(pow(ymotion, 2) + pow(xmotion, 2))); 
		*/

		/* old code for tank drive
		left_mtr1 = left - turn;
		left_mtr2 = left - turn;
		right_mtr1 = -right - turn;
		right_mtr2 = -right - turn;
		*/
		
		pros::lcd::set_text(0, "Left stick X: " + std::to_string(xmotion));
		pros::lcd::set_text(1, "Left stick Y: " + std::to_string(ymotion));
		pros::lcd::set_text(2, "LF power : " + std::to_string(mtr_lf.get_power()) + "\t RB power: "+ std::to_string(mtr_rb.get_power()));
		pros::lcd::set_text(3, "LB power: " + std::to_string(mtr_lb.get_power()) + "\t RF power: "+ std::to_string(mtr_rf.get_power()));
		pros::lcd::set_text(4, "Strafe angle: " + std::to_string(strafeAngle));

		pros::delay(20);
	}
}
