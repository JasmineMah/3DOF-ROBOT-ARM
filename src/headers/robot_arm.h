#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <math.h>
#include <algorithm>
#include <BasicLinearAlgebra.h>
#include <Adafruit_PWMServoDriver.h>

// Temporary linkage lengths that really should be remeasured, given in cm.
// TODO: Remeasure these angles.
const float L1 = 19.6;
const float L2 = 19.6;
const float L3 = 1.2345; // TO BE MEASURED

BLA::Matrix<3> forwardKinematics(BLA::Matrix<3> angles);
BLA::Matrix<3,3> estimateInitialJacobian(Adafruit_PWMServoDriver &pwm);
BLA::Matrix<3,3> recomputeJacobian(BLA::Matrix<3> angles);
bool newton(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> P_tgt, int max_iter, float threshold);

#endif