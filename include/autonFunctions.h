#pragma once
#include "api.h"

extern double inches;
extern double eP, eD, eI; // distance error, distance error rate, and  

void moveLeftSide(int distance, int power);

void moveRightSide(int distance, int power);

void moveAll(int distance, int power);

void skillsAuton();

void straightPID(double pGain, double dGain, double iGain);

void leftAuton();

void rightAuton();

void turnRight90();

void turnLeft90();

void flipRight();

void flipLeft();