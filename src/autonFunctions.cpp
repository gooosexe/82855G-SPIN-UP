#include "api.h"
#include "motorDef.h"
#include "autonFunctions.h"

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