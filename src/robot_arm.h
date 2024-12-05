#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <math.h>
#include <algorithm>
#include <BasicLinearAlgebra.h>


// TODO: Measure the linkage lengths.
extern const double L1 = 19.6; // in cm
extern const double L2 = 19.6; // in cm
extern const double L3 = 1.2345; // NEED TO MEASURE

BLA::Matrix<3> forward_kinematics(BLA::Matrix<3> angles);
BLA::Matrix<3,3> estimate_initial_jacobian();
BLA::Matrix<3,3> recompute_jacobian(BLA::Matrix<3> angles);

// DUMMY FUNCTION TO BE REPLACED
void temp_move_motors(BLA::Matrix<3> angles) {
    // REMOVED FOR NOW    
}

// DUMMY FUNCTION TO BE REPLACED
BLA::Matrix<3> temp_get_motor_angles() {
    return BLA::Matrix<3>{1., 2., 3.};
}

#endif