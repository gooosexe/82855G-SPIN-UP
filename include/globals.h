#pragma once
#include "api.h"
#include "globals.h"

using namespace pros;

// multiply this constant by the distance in inches to accurately move in inches
// this constant ONLY WORKS if the motors are set to calculate the number of rotations made
// the 3/7 is calculating for the gear ratio
extern const double inchConstant = (1/(4.125*M_PI)) * (3/7);

extern Motor mtr_lf, mtr_lb, mtr_rf, mtr_rb;

extern Motor rollerMtr;

extern Motor mtr_intake;
extern Motor flywheel;
extern Motor mtr_indexer;

extern ADIDigitalOut pn_expand, pn_expand2;