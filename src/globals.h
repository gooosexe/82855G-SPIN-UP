#pragma once
#include "api.h"
#include "globals.h"
#include "pros/misc.hpp"

using namespace pros;

extern Motor mtr_lf, mtr_lb, mtr_rf, mtr_rb;

extern Motor rollerMtr;

extern Motor mtr_intake;
extern Motor flywheel;
extern Motor mtr_indexer;

extern Controller master;

extern IMU imu_sensor;

extern ADIDigitalOut pn_expand;