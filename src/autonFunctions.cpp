#include "api.h"
#include "globals.h"
#include "autonFunctions.h"
#include "globals.h"
#include "pros/llemu.hpp"

using namespace std;
using namespace pros;

const double inchConstant = (1/(4.125*M_PI)) * (3.0/7.0);
const double errorThreshold = 0.25;
const double angleThreshold = 0.2;

double propGain = 4;
double derivGain = 0.1;
double integGain = 0.1;

double rightpE, rightdE, rightiE;
double leftpE, leftdE, leftiE;
double anglepE, angledE, angleiE;
double leftPrevE, rightPrevE, anglePrevE;
        

double PID(double propError, double derivError, double integError) { 
    double velocity;
    velocity = (propGain * propError) + (derivGain * derivError) + (integGain * integError); // proportional term, derivative term, and integral term respectively 
    return velocity;
};

void resetMotors() {
    mtr_lb.tare_position();
    mtr_rb.tare_position();
    delay(20);
}

// all distances will be inputted as number of inches
// also this is an attempt at implementing pid but it might not work so hang tight

void moveRightSide(double power) {
    mtr_rb = power;
    mtr_rf = power;
}

void moveLeftSide(double power) {
    mtr_lb = power;
    mtr_lf = power; 
}

void moveStraight(double distance) {
    resetMotors();
    leftpE = distance * inchConstant;
    rightpE = distance * inchConstant;
    leftPrevE = 0;
    rightPrevE = 0;
    lcd::print(0, "%g", leftpE);

    do {
        leftPrevE = leftpE;
        rightPrevE = rightpE;

        leftpE = distance - mtr_lb.get_position();
        rightpE = distance - mtr_rb.get_position();
        
        leftiE += leftpE;
        rightiE += rightpE;

        leftdE = leftPrevE - leftpE;
        rightdE = rightPrevE - rightpE;
        
        moveLeftSide(PID(leftpE, leftdE, leftiE));
        moveRightSide(PID(rightpE, leftdE, leftiE));
        
        lcd::print(1, 0, "%g %g %g", leftpE, leftiE, leftdE);
        lcd::print(2, 0, "%g %g %g", rightpE, rightiE, rightdE);
    } while (leftpE > 0.1 || rightpE > 0.1);  
}

void moveTurn(double angle) {
    resetMotors();
    imu_sensor.tare_rotation();

    anglepE = angle;
    anglePrevE = 0;

    if (angle )
    while (anglepE > angleThreshold) {
        anglePrevE = anglepE;
        anglepE = angle - imu_sensor.get_rotation();
        angleiE += anglepE;
        angledE = anglePrevE - anglepE;

        moveLeftSide(PID(anglepE, angledE, angleiE));
        moveRightSide(-PID(anglepE, angledE, angleiE));
    }
}

// REMEMBER TO CHANGE THE VALUES HERE BECAUSE ALL OF THEM ARE IN INCHES NOW
void skillsAuton() {

}

void leftAuton() {
    moveStraight(24);
}

void rightAuton(){
    
}


