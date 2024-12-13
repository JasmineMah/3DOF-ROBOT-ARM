#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <math.h>
#include <algorithm>
#include <BasicLinearAlgebra.h>

// Temporary linkage lengths that need to be remeasured.
extern const float L1 = 19.6; // in cm
extern const float L2 = 19.6; // in cm
extern const float L3 = 1.2345; // NEED TO MEASURE

BLA::Matrix<3> forwardKinematics(BLA::Matrix<3> angles);
BLA::Matrix<3,3> estimateInitialJacobian();
BLA::Matrix<3,3> recomputeJacobian(BLA::Matrix<3> angles);

// DUMMY FUNCTION TO BE REPLACED
void temp_move_motors(BLA::Matrix<3> angles) {
    // REMOVED FOR NOW    
}

// DUMMY FUNCTION TO BE REPLACED
BLA::Matrix<3> temp_get_motor_angles() {
    return BLA::Matrix<3>{1., 2., 3.};
}

#endif