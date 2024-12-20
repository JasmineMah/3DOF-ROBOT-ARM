#include <math.h>
#include "headers/robot_arm.h"
#include "headers/math_implementations.h"
#include "headers/motors.h"


/// @brief Calculates the forward kinematics of the arm using the Denavit-Hartenberg convention.
/// It is assumed that the motors rotate above the Z, Y, and Z-axes, respectively.
/// @param angles Motor angles.
/// @return The point.
BLA::Matrix<3> forwardKinematics(const BLA::Matrix<3> angles) {
    float theta1 = angles(0) * DEG_TO_RAD;
    float theta2 = angles(1) * DEG_TO_RAD;
    float theta3 = angles(2) * DEG_TO_RAD;

    // 2DOF arm position for the elbow and wrist in the XZ plane
    // local coordinates from second joint
    float x_l = L2 * cos(theta2) + L3 * cos(theta2 + theta3);
    float z_l = L2 * sin(theta2) + L3 * sin(theta2 + theta3);

    // apply rotation from the first joint (rotation along the z-axis)
    float x_w = cos(theta1) * x_l;
    float y_w = sin(theta1) * x_l;
    float z_w = L1 + z_l;

    return BLA::Matrix<3>{x_w, y_w, z_w};
}


/// @brief Estimates the initial Jacobian using a predefined set of motor angles.
/// @return The initial Jacobian estimate.
BLA::Matrix<3,3> estimateInitialJacobian(Adafruit_PWMServoDriver &pwm) {
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

    last_P = forwardKinematics(getMotorAngles());

    for (int i = 0; i < J.Cols; i++) {

        float delta_theta_i = delta_angles(i, i);
        moveMotor(pwm, i, delta_theta_i);

        P = forwardKinematics(getMotorAngles());
        dP = P - last_P;

        J(0, i) = dP(0) / delta_theta_i;
        J(1, i) = dP(1) / delta_theta_i;
        J(2, i) = dP(2) / delta_theta_i;

        last_P = P;

        delay(500);
    }

    return J;
}

/// @brief Recomputes the Jacobian based on the current motor angles.
/// @param angles Current motor angles.
/// @return The updated Jacobian.
BLA::Matrix<3,3> recomputeJacobian(const BLA::Matrix<3> angles) {
    
    float theta1 = angles(0) * DEG_TO_RAD;
    float theta2 = angles(1) * DEG_TO_RAD;
    float theta3 = angles(2) * DEG_TO_RAD;

    // Simplification
    float c1 = cos(theta1), s1 = sin(theta1);
    float c2 = cos(theta2), s2 = sin(theta2);
    float c23 = cos(theta2 + theta3), s23 = sin(theta2 + theta3);

    BLA::Matrix<3,3> J;

    // intermediate terms; (x, z) relative to the 2DOF arm
    float x_part = L2 * c2 + L3 * c23;
    float z_part = L2 * s2 + L3 * s23;

    J = {
        -s1 * x_part, -c1 * z_part,       -c1 * L3 * s23,
         c1 * x_part, -s1 * z_part,       -s1 * L3 * s23,
        0,            L2 * c2 + L3 * c23, L3 * c23
    };

    return J;
}

/// @brief Performs Newton's method for inverse kinematics. 
/// @param pwm Servo driver.
/// @param P_tgt Target point.
/// @param max_iter Max number of iterations allowed.
/// @param threshold Error allowed in X units.
/// @return Whether or not the movement was successful.
bool newton(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> P_tgt, int max_iter, float threshold = 2.5f) {

    int k = 0;
    BLA::Matrix<3> E, P, Q, dQ; // error, end effector point, motor angles, delta
    BLA::Matrix<3, 3> J;

    Q = getMotorAngles();
    J = estimateInitialJacobian(pwm);

    while (k < max_iter) {

        // Calculate error.
        P = forwardKinematics(Q);
        E = P_tgt - P;

        // Are we close enough to the target?
        if (calculateNorm(E) < threshold) {
            Serial.println(String("Reached target in ") + k + String(" iterations!")); // TODO: Move this to an LCD display.
            return true;
        }

        // Obtain the change in angles.
        if (isJacobianSingular(J)) {
            Serial.println("Singular Jacobian!"); // TODO: Move this to an LCD display.
            return false;
        } else {
            dQ = BLA::Inverse(J) * E;
        }

        // Move motors and update the estimate of joint angles.
        moveMotors(pwm, dQ);
        Q += dQ;
        J = recomputeJacobian(Q);        
        k++;

        delay(150);
    }

    return false;
}