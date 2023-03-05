#pragma once
#include "api.h"

// multiply this constant by the distance in inches to accurately move in inches
// this constant ONLY WORKS if the motors are set to calculate the number of rotations made
// the 3/7 is calculating for the gear ratio
extern const double inchConstant;
extern const double errorThreshold, angleThreshold;
extern double propGain, derivGain, integGain;
extern double angPropGain, angDerivGain, angIntegGain;   
extern double rightpE, rightdE, rightiE;
extern double leftpE, leftdE, leftiE;
extern double anglepE, angledE, angleiE;
extern double leftPrevE, rightPrevE, anglePrevE;
extern double angleError, curAngle;

void moveLeftSide(int power);

void resetMotors();

void moveRightSide(int power);

void moveStraight(double distance);

void moveTurn(double angle);

void skillsAuton();

double PID(double propError, double derivError, double integError);

double angPID(double angPropError, double angDerivError, double andIntegError);

void turningsex(double angle);

void leftAuton();

void rightAuton();

// void turnRight90();

// void turnLeft90();

// void flipRight();

// void flipLeft();