// implement the lab 2 code
// specifically part 3ai; numerical (Newton's or Broyden's method)

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>
#include <BasicLinearAlgebra.h>

#include <vector>
#include <robot_arm.h>

void setup() {

}

/// @brief Calculates the norm of a 1x3 vector.
/// @param v 
/// @return The norm.
float calculateNorm(BLA::Matrix<3> v) {
    BLA::Matrix<1> inner = ~v * v;
    return sqrt(inner(0));
}

/// @brief Performs Newton's method for inverse kinematics. 
/// @param Xt Target point.
/// @param max_iter Max number of iterations allowed.
/// @param threshold Error allowed in X unit.
/// @return Whether or not the movement was successful.
bool newton(BLA::Matrix<3> Xt, int max_iter, double threshold) {

    // --- algorithm ---
    // definitions:
    // X: double[] = [x y z]
    // Q: double[] = [m1, m2, ..., mn]
    // k: int
    // max_iter: int
    // threshold: double

    // while k < max_iter
    // E = X* - Xk
    // if norm(E) < threshold EXIT
    // dQk = J^-1 x E
    // Qk+1 = Qk + dQk
    // updateJacobian()
    // end while

    int k = 0;
    BLA::Matrix<3> E, X, Q, dQ;
    BLA::Matrix<3, 3> J;
    Q = {1.0f, 2.0f, 3.0f};

    J = estimateInitialJacobian();

    while (k < max_iter) {
        // get angles from the motor encoders
        // Q = getMotorAnglesOrSomething();

        // calculate error
        X = forwardKinematics(Q);
        E = Xt - X;

        // check threshold
        if (calculateNorm(E) < threshold) {
            return true;
        }

        // get dQ from J
        // NOTE: this can get really bad; singular matrix moment
        try {
            dQ = BLA::Inverse(J) * E; // TODO: Handle the case where the matrix is singular.
        } catch(...) {
            Serial.println("Uh oh");
            return false;
        }

        // move motors by dQ
        // this can be done in parallel, maybe
        // moveMotorAnglesOrSomething(dQ);

        // recompute the Jacobian
        // J = recomputeJacobian(Q);
        k++;
    }

    return false;


}

/*
NOTE: Angles are to be converted to RADIAN only when we intend to
calculate them. Otherwise, they should be in degrees.
*/

void loop() {

}