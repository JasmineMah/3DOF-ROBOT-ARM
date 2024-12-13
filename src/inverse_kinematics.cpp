#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>
#include <BasicLinearAlgebra.h>

#include <vector>
#include <robot_arm.h>
#include <motors.h>
#include <math_implementations.h>

/*
Inverse kinematics implementation.

NOTE: Angles are to be converted to RADIAN only when we intend to
calculate them. Otherwise, they should be in degrees.
*/


/// @brief Performs Newton's method for inverse kinematics. 
/// @param P_tgt Target point.
/// @param max_iter Max number of iterations allowed.
/// @param threshold Error allowed in X units.
/// @return Whether or not the movement was successful.
bool newton(BLA::Matrix<3> P_tgt, int max_iter, float threshold = 2.5f) {

    int k = 0;
    BLA::Matrix<3> E, P, Q, dQ; // error, end effector point, motor angles, delta
    BLA::Matrix<3, 3> J;

    Q = getMotorAngles();
    J = estimateInitialJacobian();

    while (k < max_iter) {

        // calculate the error
        P = forwardKinematics(Q);
        E = P_tgt - P;

        // are we close enough to the target?
        if (calculateNorm(E) < threshold) {
            Serial.println(String("Reached target in ") + k + String(" iterations!"));
            return true;
        }

        // get dQ from J
        if (isJacobianSingular(J)) {
            Serial.println("Singular Jacobian!");
            return false;
        } else {
            dQ = BLA::Inverse(J) * E;
        }

        // move motors by dQ, then update the estimate of joint angles
        // moveMotors(dQ);
        Q += dQ;

        J = recomputeJacobian(Q);
                
        k++;
        delay(150);
    }

    return false;


}

void setup() {

}

void loop() {

}
