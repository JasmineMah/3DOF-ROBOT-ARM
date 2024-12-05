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
extern const double L1 = 19.6; // in cm
extern const double L2 = 19.6; // in cm
extern const double L3 = 1.2345; // NEED TO MEASURE

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
BLA::Matrix<3> forward_kinematics(BLA::Matrix<3> angles) {
    double theta1 = angles(0) * DEG_TO_RAD;
    double theta2 = angles(1) * DEG_TO_RAD;
    double theta3 = angles(2) * DEG_TO_RAD;

    // 2DOF arm position for the elbow and wrist in the XZ plane
    // local coordinates from second joint
    double x_l =      L2 * cos(theta2) + L3 * cos(theta2 + theta3);
    double z_l = L1 + L2 * sin(theta2) + L3 * sin(theta2 + theta3);

    // apply rotation from the first joint (rotation along the z-axis)
    double x_w = cos(theta1) * x_l;
    double y_w = sin(theta1) * x_l;
    double z_w = z_l;

    return BLA::Matrix<3>{x_w, y_w, z_w};
}


BLA::Matrix<3,3> estimate_initial_jacobian() {
    // TODO: might need motor control reference
    BLA::Matrix<3, 3> J = BLA::Eye<3,3>();

    BLA::Matrix<3,3> delta_angles;
    BLA::Matrix<3> P, last_P, dP;

    // first column: first motor
    // second column: second motor
    // third column: third motor

    // movements to move the initial jacobian in degrees
    delta_angles = {
        3, 0, 0,
        0, 5, 0,
        0, 0, 6
    };

    last_P = forward_kinematics(temp_get_motor_angles());

    for (int i = 0; i < J.Cols; i++) {
        temp_move_motors(
            BLA::Matrix<3>{delta_angles(i, 0), delta_angles(i, 1), delta_angles(i, 2)}
        );
        P = forward_kinematics(temp_get_motor_angles());
        dP = P - last_P;

        double delta_theta_i = delta_angles(i, i);
        J(0, i) = dP(0) / delta_theta_i;
        J(1, i) = dP(1) / delta_theta_i;
        J(2, i) = dP(2) / delta_theta_i;

        last_P = P;
    }

    return J;
}

BLA::Matrix<3,3> recompute_jacobian(BLA::Matrix<3> angles) {
    
    double theta1 = angles(0) * DEG_TO_RAD;
    double theta2 = angles(1) * DEG_TO_RAD;
    double theta3 = angles(2) * DEG_TO_RAD;

    BLA::Matrix<3,3> J;

    double c1 = cos(theta1), s1 = sin(theta1);
    double c2 = cos(theta2), s2 = sin(theta2);
    double c23 = cos(theta2 + theta3), s23 = sin(theta2 + theta3);

    double x_part = L2 * c2 + L3 * c23;

    // compute partial derivatives
    double dx_dtheta1 = -s1 * x_part;
    double dy_dtheta1 = c1 * x_part;
    double dz_dtheta1 = 0;

    double dx_dtheta2 = -c1 * (L2 * s2 + L3 * s23);
    double dy_dtheta2 = -s1 * (L2 * s2 + L3 * s23);
    double dz_dtheta2 = L2 * c2 + L3 * c23;

    double dx_dtheta3 = -c1 * L3 * s23;
    double dy_dtheta3 = -s1 * L3 * s23;
    double dz_dtheta3 = L3 * c23;

    J = {
        dx_dtheta1, dx_dtheta2, dx_dtheta3,
        dy_dtheta1, dy_dtheta2, dy_dtheta3,
        dz_dtheta1, dz_dtheta2, dz_dtheta3
    };

    return J;
}

void temp_move_motors(BLA::Matrix<3> angles) {
    // REMOVED FOR NOW    
    // for (int i = 0; i < angles.Cols; i++) {
    //     if (angles(i) != NAN) {
    //         // TODO: move motor i (this can be asynchronous) by the given angle
    //     }
    // }

    // return;
}

BLA::Matrix<3> temp_get_motor_angles() {
    // TODO: read from the encoders
    return BLA::Matrix<3> {1., 2., 3.};
}

#endif