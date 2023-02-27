#include "api.h"
#include "motorDef.h"
#include "autonFunctions.h"

double inches = 200;

double kP = 0.7;
double kI = 0;
double kD = 0;

double turn_kP = 0.7;
double turn_kI = 0;
double turn_kD = 0;

    // PID VARIABLES
int error;
int prevError;
int deriv;
int totalError;
int targetPos = 100;

int turn_error, turn_prevError, turn_deriv, turn_totalError, turn_targetPos;

bool enablePID = true;
bool resetDriveSensors = false;


int drivePID() {
    while(enablePID) {
        if (resetDriveSensors) {
            resetDriveSensors = false;
            mtr_lf.tare_position();
            mtr_lb.tare_position();
            mtr_rf.tare_position();
            mtr_rb.tare_position();
        }

        int lfPos = mtr_lf.get_position();
        int lbPos = mtr_lb.get_position();
        int rfPos = mtr_rf.get_position();
        int rbPos = mtr_rb.get_position();

            int avgPos = (lfPos + lbPos + rfPos + rbPos)/4;

            // POTENTIAL
            error = avgPos - targetPos;

            // DERIVATIVE
            deriv = error - prevError;

            // INTEGRAL
            totalError += error;

            double latMotion = error * kP + deriv * kD + totalError * kI;

            // turn difference
            int turnDifference = (((lfPos -rfPos)/2) + ((lbPos - rbPos)/2))/2;

            // turn potential
            turn_error = turnDifference - targetPos;

            // turn derivative
            turn_deriv = turn_error - turn_prevError;

            // integral
            turn_totalError += turn_error;

            double turnMotion = turn_error * kP + turn_deriv * kD + turn_totalError * kI;

            mtr_lf.move_voltage(latMotion + turnMotion);
            mtr_lb.move_voltage(latMotion + turnMotion);
            mtr_rf.move_voltage(latMotion - turnMotion);
            mtr_rb.move_voltage(latMotion - turnMotion);

            prevError = error;
            turn_prevError = turn_error;

            pros::delay(20);
        }
    return 1;
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

void testPID() {
    drivePID();

    targetPos = 1000;
    turn_targetPos = 600;
}

void skillsAuton() {
    
    //move to roller
    moveAll(-inches*13.5, 100);
    pros::delay(2000);
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
    moveLeftSide(-865, 80); // turn (greater value, towards alliance low goal)
    moveRightSide(865, 80);
    pros::delay(1000);
    mtr_intake = -127;
    moveAll(-3800, 127);
    pros::delay(4000);
    // shoot high goals
    turnLeft90();  
    pros::delay(1500);
    moveAll(100, 127);
    mtr_indexer.move_relative(3500, 90);
    pros::delay(3000);
    // intake stack
    moveAll(-100, 127);
    moveLeftSide(-585, 80);
    moveRightSide(585, 80);
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
    moveAll(500, 80);
    pros::delay(500);
    moveLeftSide(250, 80); // turn clockwise
    moveRightSide(-250, 80);
    pros::delay(300);
}

void leftAuton() {
    // LS auto                                                                                                                               

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

    // intake stack and shoot
    // moveLeftSide(725, 80);
    // moveRightSide(-725, 80);
    // pros::delay(1500);
    // mtr_intake = -127;
    // moveAll(-3000 , 127);
    // pros::delay(3000);
    // moveLeftSide(585, 80);
    // moveRightSide(-585, 80);                                                 
    // pros::delay(1700);                          
    // flywheel = 127;
    // pros::delay(1500);
    // moveAll(100, 127);
    // mtr_indexer.move_relative(3500, 127);
    // pros::delay(3000);
    // moveAll(-500, 80);
    // flywheel = 0;
}

void rightAuton(){
    mtr_lf.move_relative(inches*13, 100); // Move forwards
    mtr_lb.move_relative(inches*13, 100);
    mtr_rf.move_relative(-inches*13, 100);
    mtr_rb.move_relative(-inches*13, 100);
    pros::delay(100);


    pros::delay(1000);


    mtr_lf.move_relative(-550, 50); // Turn clockwise
    mtr_lb.move_relative(-550, 50);
    mtr_rf.move_relative(-550, 50);
    mtr_rb.move_relative(-550, 50);


    pros::delay(1000);


    mtr_lf.move_relative(inches*2, 80); // Move forwards
    mtr_lb.move_relative(inches*2, 80);
    mtr_rf.move_relative(-inches*2, 80);
    mtr_rb.move_relative(-inches*2, 80);


    pros::delay(1000);


    rollerMtr.move_relative(470, 127);


    pros::delay(1000);


    mtr_lf.move_relative(-inches*3, 80); // Move back
    mtr_lb.move_relative(-inches*3, 80);
    mtr_rf.move_relative(inches*3, 80);
    mtr_rb.move_relative(inches*3, 80);


    pros::delay(1000);


    mtr_lf.move_relative(620, 80); // Turn clockwise
    mtr_lb.move_relative(620, 80);
    mtr_rf.move_relative(620, 80);
    mtr_rb.move_relative(620, 80);


    pros::delay(500);


    moveAll(550, 80);


    pros::delay(200);


    flywheel = 115;


    pros::delay(2000);


    mtr_indexer.move_relative(5000, 127);


    pros::delay(2000);


    moveLeftSide(-820, 80);
    moveRightSide(820, 80);


    pros::delay(1000);


    mtr_intake = -127;


    moveAll(-3500, 127);
   
    pros::delay(3500);


    moveLeftSide(-585, 80);
    moveRightSide(585, 80);  


    pros::delay(1500);


    moveAll(100, 127);


    mtr_indexer.move_relative(3500, 127);


}


