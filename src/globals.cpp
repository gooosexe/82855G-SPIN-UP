#include "api.h"
#include "globals.h"
#include "pros/adi.hpp"

Motor mtr_lf(15, E_MOTOR_GEARSET_06, false);
Motor mtr_lb(16, E_MOTOR_GEARSET_06, false);
Motor mtr_rf(19, E_MOTOR_GEARSET_06, false);
Motor mtr_rb(20, E_MOTOR_GEARSET_06, false);

Motor rollerMtr(3, E_MOTOR_GEARSET_36, false);

Motor mtr_intake(9, E_MOTOR_GEARSET_06, false);
Motor flywheel(10, E_MOTOR_GEARSET_06, false);
Motor mtr_indexer(7, E_MOTOR_GEARSET_18, false);

ADIDigitalOut pn_expand('A');
ADIDigitalOut pn_expand2('B');
