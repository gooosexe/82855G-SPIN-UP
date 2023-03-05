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

void turningsex(double angle) {
    // resetMotors();
    // lcd::clear();
    // double error = angle;
    // do {
    //     moveRightSide(80);
    //     moveLeftSide(-80);

    //     lcd::set_text(0, to_string(error));
    //     printf("IMU rotation: %f degrees\n", imu_sensor.get_heading());
    //     error = angle - imu_sensor.get_heading();

    //     pros::delay(50);
    // }
    // while (fabs(error) > 0.1);
    curAngle = imu_sensor.get_heading();
    if (curAngle - angle > 0) {
        do {
            angleError = imu_sensor.get_heading() - angle;
            moveRightSide(40);
            moveLeftSide(-40);
            master.print(0, 0, "Angle Error: %f degrees", angleError);
            delay(20);
        } while (angleError > 0.25);

        resetMotors();
    } else {
        do {
            angleError = angle - imu_sensor.get_heading();
            moveRightSide(-40);
            moveLeftSide(40);
            master.print(0, 0, "Angle Error: %f degrees", angleError);
            delay(20);
        } while (angleError > 0.25);

        resetMotors();
    }
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

}

void leftAuton() {
    
    // moveStraight(24);
    //delay(250);
    turningsex(90);
    delay(500);
    turningsex(-90);
}

void rightAuton(){
    
}


