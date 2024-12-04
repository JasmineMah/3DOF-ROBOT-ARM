#ifndef ROBOT_ARM
#define ROBOT_ARM

#include <math.h>
#include <algorithm>
#include <BasicLinearAlgebra.h>

struct Point3D {
    double x;
    double y;
    double z;
};

// TODO: Measure the linkage lengths.
extern const double L1 = 1.2345;
extern const double L2 = 1.2345;
extern const double L3 = 1.2345;

double deg2rad(double theta) {
    return theta * DEG_TO_RAD;
}

double rad2deg(double theta) {
    return theta * RAD_TO_DEG;
}

/// @brief Calculates the forward kinematics of the arm using the Denavit-Hartenberg convention.
/// It is assumed that the motors rotate above the Z, Y, and Z-axes, respectively.
/// @param angles Motor angles.
/// @return The point.
BLA::Matrix<3> forwardKinematics(std::vector<double> angles) {
    std::for_each(angles.begin(), angles.end(), &deg2rad);

    double x = L1 * cos(angles[0]) + L2 * cos(angles[0]) * cos(angles[1]) +
               L3 * cos(angles[0]) * cos(angles[1] + angles[2]);

    double y = L1 * sin(angles[0]) + L2 * sin(angles[0]) * cos(angles[1]) +
               L3 * sin(angles[0]) * cos(angles[1] + angles[2]);

    double z = L2 * sin(angles[1]) + L3 * sin(angles[2] + angles[3]);

    return BLA::Matrix<3>{x, y, z};
}


BLA::Matrix<3,3> estimateInitialJacobian() {
    BLA::Matrix<3, 3> J = BLA::Eye<3,3>();
    // BLA::Matrix<3,3> X = forwardKinematics();
    // use forward kinematics to estimate the change.
    double dX, dY, dZ;

    // first column first motor

    // second column second motor

    // third column third motor
}

BLA::Matrix<3,3> recomputeJacobian(BLA::Matrix<3> angles) {
    
    BLA::Matrix<3,3> J;

    // TODO: Convert to radian

    double theta1 = angles(0), theta2 = angles(1), theta3 = angles(2);

    double c1 = cos(theta1), s1 = sin(theta1);
    double c2 = cos(theta2), s2 = sin(theta2);
    double c23 = cos(theta2 + theta3), s23 = sin(theta2 + theta3);

    J = {
        -s1 * (L1 + L2 * c2 + L3 * c23), -c1 * (L2 * s2 + L3 * s23), -L3 * c1 * s23,
        c1 * (L1 + L2 * c2 + L3 * c23),  -s1 * (L2 * s2 + L3 * s23), -L3 * s1 * s23,
        0,                               L2 * c2 + L3 * c23,         L3 * c23
    };

    return J;
}

#endif