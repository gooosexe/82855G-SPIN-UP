#include "api.h"
#include "globals.h"
#include "autonFunctions.h"
#include "globals.h"
#include "pros/llemu.hpp"
#include "pros/screen.hpp"

using namespace std;
using namespace pros;

const double inchConstant = (1/(4.125*M_PI)) * (7.0/3.0);
const double errorThreshold = 0.25;
const double angleThreshold = 0.2;

double propGain = 29;
double derivGain = 6;
double integGain = 0.05;

double rightpE, rightdE, rightiE;
double leftpE, leftdE, leftiE;
double anglepE, angledE, angleiE;
double leftPrevE, rightPrevE, anglePrevE;

double angPropGain = 29;
double angDerivGain = 6;
double angIntegGain = 0.05;

double angleError, curAngle;

double PID(double propError, double derivError, double integError) { 
    double velocity;
    velocity = (propGain * propError) + (derivGain * derivError) + (integGain * integError); // proportional term, derivative term, and integral term respectively 
    return velocity;
};

double angPID(double angPropError, double angDerivError, double andIntegError) {
    double velocity;
    velocity = (angPropGain * angPropError) + (angDerivError * angDerivError) + (andIntegError * angIntegGain);
    return velocity;
}

void resetMotors() {
    mtr_lb.tare_position();
    mtr_rb.tare_position();
    mtr_lb = 0;
    mtr_rb = 0;
    mtr_lf = 0;
    mtr_rf = 0;
    delay(100);
}

// all distances will be inputted as number of inches
// also this is an attempt at implementing pid but it might not work so hang tight

void moveRightSide(int power) {
    mtr_rb = power;
    mtr_rf = power;
}

void moveLeftSide(int power) {
    mtr_lb = power;
    mtr_lf = power; 
}

void moveStraight(double distance) {
    resetMotors();
    leftpE = distance * inchConstant;
    rightpE = distance * inchConstant;
    leftPrevE = 0;
    rightPrevE = 0;
    lcd::print(0, "%f", leftpE);

    do {
        leftPrevE = leftpE;
        rightPrevE = rightpE;


        leftpE = distance*inchConstant - mtr_lb.get_position();
        rightpE = distance*inchConstant - mtr_rb.get_position();
        
        leftiE += leftpE;
        rightiE += rightpE;

        leftdE = leftPrevE - leftpE;
        rightdE = rightPrevE - rightpE;
        
        moveLeftSide(PID(leftpE, leftdE, leftiE));
        moveRightSide(PID(rightpE, rightdE, rightiE));
        
        lcd::set_text(1, to_string(leftpE) + " " + to_string(leftiE) + " " + to_string(leftdE));
        lcd::set_text(2, to_string(rightpE) + " " + to_string(rightiE) + " " + to_string(rightdE));
        lcd::set_text(3, to_string(mtr_lb.get_position()));
        lcd::set_text(4, to_string(mtr_rb.get_position()));

        delay(50);
    } while (leftpE > 0.1 || rightpE > 0.1); 

    resetMotors();
}

void moveBack(double distance) {
    resetMotors();
    leftpE = distance * inchConstant;
    rightpE = distance * inchConstant;
    leftPrevE = 0;
    rightPrevE = 0;
    lcd::print(0, "%f", leftpE);

    do {
        leftPrevE = leftpE;
        rightPrevE = rightpE;


        leftpE = distance*inchConstant + mtr_lb.get_position();
        rightpE = distance*inchConstant + mtr_rb.get_position();
        
        leftiE += leftpE;
        rightiE += rightpE;

        leftdE = leftPrevE - leftpE;
        rightdE = rightPrevE - rightpE;
        
        moveLeftSide(-PID(leftpE, leftdE, leftiE));
        moveRightSide(-PID(rightpE, rightdE, rightiE));
        
        lcd::set_text(1, to_string(leftpE) + " " + to_string(leftiE) + " " + to_string(leftdE));
        lcd::set_text(2, to_string(rightpE) + " " + to_string(rightiE) + " " + to_string(rightdE));
        lcd::set_text(3, to_string(mtr_lb.get_position()));
        lcd::set_text(4, to_string(mtr_rb.get_position()));

        delay(50);
    } while (leftpE > 0.1 || rightpE > 0.1); 

    resetMotors();
}

// void turningsex(double angle) {
//     // resetMotors();
//     // lcd::clear();
//     // double error = angle;
//     // do {
//     //     moveRightSide(80);
//     //     moveLeftSide(-80);

//     //     lcd::set_text(0, to_string(error));
//     //     printf("IMU rotation: %f degrees\n", imu_sensor.get_heading());
//     //     error = angle - imu_sensor.get_heading();

//     //     pros::delay(50);
//     // }
//     // while (fabs(error) > 0.1);
//     curAngle = imu_sensor.get_heading();
//     if (curAngle - angle > 0) {
//         do {
//             angleError = imu_sensor.get_heading() - angle;
//             moveRightSide(40);
//             moveLeftSide(-40);
//             master.print(0, 0, "Angle Error: %f degrees", angleError);
//             delay(20);
//         } while (angleError > 0.25);

//         resetMotors();
//     } else {
//         do {
//             angleError = angle - imu_sensor.get_heading();
//             moveRightSide(-40);
//             moveLeftSide(40);
//             master.print(0, 0, "Angle Error: %f degrees", angleError);
//             delay(20);
//         } while (angleError > 0.25);

//         resetMotors();
//     }
    /*do {
        angleError = angle - (imu_sensor.get_heading() - curAngle);
        moveRightSide(-40);
        moveLeftSide(40);
        lcd::print(0, "IMU heading: %f degrees", imu_sensor.get_heading());
        // master.print(0, 0, "IMU heading: %f degrees", imu_sensor.get_heading());
        master.print(0, 0, "Angle Error: %f degrees", angleError);
        delay(5);
        
    } while (fabs(angleError) > 0.5);

    /*while (true) {
        lcd::print(0, "IMU heading: %f degrees", imu_sensor.get_heading());
        delay(20);
    }*/


// void moveStraight(double targetDis) { //TODO TUNE + SPEED CAP TO ALLOW FOR LEFT RIGHT ADJUSTMENT OVERRIDE

// 	// Encoder objects 
// 	mtr_lf.tare_position();
// 	mtr_rf.tare_position();
// 	pros::delay(1500); // Wait for encoders to reset

// 	// Constants
// 	float circum = 4.125 * M_PI;
// 	float gearRatio = (float)3 / (float)7;
// 	float targetRot = targetDis / circum;
// 	float curRotRight;
// 	float curErrorRight;
// 	float prevErrorRight = 0;
// 	float totalErrorRight = 0;
// 	float curRotLeft;
// 	float curErrorLeft;
// 	float prevErrorLeft = 0;
// 	float totalErrorLeft = 0;
// 	float allowedError = 0.5; // 0.25
// 	float kP = 4.2; // 4.25 is original
// 	float kI = 0.025; // 0.05 is original
// 	float kD = 1.25; // 1.75 is og 
// 	float powerRight;
// 	float powerLeft;
// 	float maxPower = 100;

// 	master.print(0, 0, "Translate %d", targetDis);
// 	do { 
		
// 		curRotRight = (mtr_rf.get_position()) * gearRatio; 
// 		curErrorRight = targetDis - (curRotRight * circum);
// 		if (fabs(curErrorRight) < 5) {
// 			totalErrorRight += curErrorRight;
// 		}
// 		powerRight = curErrorRight * kP + totalErrorRight * kI + (curErrorRight - prevErrorRight) * kD;
// 		prevErrorRight = curErrorRight;

// 		curRotLeft = (mtr_lf.get_position()) * gearRatio;
// 		curErrorLeft = targetDis - (curRotLeft * circum);
// 		if (fabs(curErrorLeft) < 5) {
// 			totalErrorLeft += curErrorLeft;
// 		}
// 		powerLeft = curErrorLeft * kP + totalErrorLeft * kI + (curErrorLeft - prevErrorLeft) * kD;
// 		prevErrorLeft = curErrorLeft;

// 		// Scaling for the motors in case they are too powerful
// 		if (fabs(powerLeft) > maxPower && std::max(fabs(powerRight), fabs(powerLeft)) == fabs(powerLeft)) {
// 			powerRight = maxPower * fabs(powerRight / powerLeft) * (fabs (powerRight) / powerRight); 
// 			powerLeft = maxPower * (fabs (powerLeft) / powerLeft); 
// 		} else if (fabs(powerRight) > maxPower && std::max(fabs(powerRight), fabs(powerLeft)) == fabs(powerRight)) {
// 			powerLeft = maxPower * fabs(powerLeft / powerRight) * (fabs (powerLeft) / powerLeft); 
// 			powerRight = maxPower *  (fabs (powerRight) / powerRight);
// 		}

// 		if (fabs(curErrorRight) > allowedError) {
// 			moveRightSide(-powerRight);
// 		}
// 		if (fabs(curErrorLeft) > allowedError) {
// 			moveLeftSide(-powerLeft);
// 		}
		
// 		pros::delay(20);
// 		pros::lcd::print(0, "R: %.2f", curRotRight);
// 		pros::lcd::print(1, "L: %.2f", curRotLeft);
// 		pros::lcd::print(2, "R error: %.2f", curErrorRight);
// 		pros::lcd::print(3, "L error: %.2f", curErrorLeft);


// 	} while(fabs(curErrorRight) > allowedError || fabs(curErrorLeft) > allowedError || fabs(mtr_lf.get_actual_velocity()) > 20 || fabs(mtr_rf.get_actual_velocity()) > 20);
// 	master.print(0, 0, "Translate Done");
//     moveLeftSide(0);
//     moveRightSide(0);

// }


void PIDturning(double targetHeading) {
    float kP = 4.2; // 1.175;   1.8
	float kI = 0.03; // 0.03
	float kD = 6;// 1
	float curHeading;
	float curError = 0;
	float prevError = 0;
	float totalError = 0;
	float allowedErr = 1; // pervious was 0.75
	float power;
	float minPower = 10;

	do {
		curHeading = imu_sensor.get_heading();
		pros::lcd::print(0, "CurHeading %f", curHeading);
		pros::lcd::print(1, "TargetHeading %f", targetHeading);

		curError = fabs(targetHeading - curHeading);
		if (curError > 180) {
			curError = 360 - curError;
		}
		bool rotClockwise = (curHeading < targetHeading) ? (targetHeading - curHeading < 180) : (curHeading - targetHeading > 180);
		if (!rotClockwise) {
			curError *= -1;
		}
		if (fabs(curError) < 10) {
			totalError += curError;
		}		
		power = -(curError * kP + totalError * kI + (curError - prevError) * kD);
		prevError = curError;
		if (fabs(curError) > allowedErr) {
			moveLeftSide(-power);
			moveRightSide(power);
		}
		// add a min power
		if (fabs(power) < minPower && fabs(curError) > allowedErr) {
			moveLeftSide(-minPower * (fabs(power) / power));
			moveRightSide(minPower * (fabs(power) / power));
		}

		pros::delay(20);                                                                                                                                                                                                                                                                                                             \
	} while(fabs(curError) > allowedErr || fabs(mtr_lf.get_actual_velocity()) > 15 || fabs(mtr_lb.get_actual_velocity()) > 15); // used to be 3
	moveLeftSide(0);
	moveRightSide(0);
}		

void moveTurn(double angle) {
    resetMotors();
    lcd::clear();
    anglepE = angle;
    anglePrevE = 0;
    double curAngle = imu_sensor.get_heading();

    do {
        anglePrevE = anglepE;
        anglepE = (angle + curAngle) - imu_sensor.get_heading();
        angleiE += anglepE;
        angledE = anglePrevE - anglepE;

        moveLeftSide(angPID(anglepE, angledE, angleiE));
        moveRightSide(-angPID(anglepE, angledE, angleiE));

        lcd::set_text(1, to_string(anglepE) + " " + to_string(angleiE) + " " + to_string(angledE));
        lcd::set_text(2, to_string(imu_sensor.get_heading()));

        delay(50);
    } while (fabs(anglepE) > angleThreshold);
}

// REMEMBER TO CHANGE THE VALUES HERE BECAUSE ALL OF THEM ARE IN INCHES NOW
void skillsAuton() {

// //roller
//     moveStraight(-12);
//     pros::delay(100);
//     pros::delay(1000);
//     turningsex(90);
//     pros::delay(500);
//     moveStraight(-2);
//     pros::delay(500);
//     rollerMtr.move_relative(470, 127);
//     pros::delay(1000);

//     // low goals
//     moveStraight(3);
//     pros::delay(1000);
//     turningsex(95);
//     pros::delay(200);
//     flywheel = 115;
//     pros::delay(3000);
//     mtr_indexer.move_relative(3000, 127);

//     // intake
//     turningsex(300);
//     delay(500);
//     mtr_intake = -127;
//     moveStraight();
//     delay(500);

//     // shoot high goals
//     turningsex(-90);
}

void testPID() {

    moveStraight(18);

    pros::delay(500);

    PIDturning(90);
    pros::delay(500);

    moveStraight(18);
    pros::delay(500);

    PIDturning(180);
    pros::delay(500);

    moveStraight(18);
    pros::delay(500);

    PIDturning(270);
    pros::delay(500);

    moveStraight(18);
    pros::delay(500);

    PIDturning(0);
    pros::delay(500);
    
}

void leftAuton() {

// roller 1
    moveBack(10);
    pros::delay(500);
    rollerMtr.move_relative(480, 127);
    pros::delay(1000);
    moveStraight(10);
    pros::delay(500);

  // 2 low goals
    PIDturning(90);
    pros::delay(500);
    moveStraight(4);
    flywheel = 90;
    pros::delay(2000);
    mtr_indexer.move_relative(2000, 80);
    pros::delay(1000);
    moveBack(4);

// intake stack and shoot
    // PIDturning(225);
    // pros::delay(1500);
    // mtr_intake = -127;
    // moveBack(31);
    // pros::delay(3000);
    // PIDturning(310);
    // pros::delay(1700);
    // flywheel = 127;
    // pros::delay(1500);
    // moveStraight(2);
    // mtr_indexer.move_relative(3500, 127);
    // pros::delay(3000);
    // moveBack(2);
    // flywheel = 0;
}

void rightAuton() {
    moveBack(18);
    PIDturning(90);
    moveBack(2);
    rollerMtr.move_relative(480, 127);
    moveStraight(2);
    PIDturning(0);
    moveStraight(20);
    flywheel = 80;
    mtr_indexer.move_relative(3000, 100);
}
