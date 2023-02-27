#pragma once
#include "api.h"

extern double inches;

void moveLeftSide(int distance, int power);

void moveRightSide(int distance, int power);

void moveAll(int distance, int power);

void skillsAuton();

int drivePID();

void leftAuton();

void rightAuton();

void turnRight90();

void turnLeft90();

void flipRight();

void flipLeft();