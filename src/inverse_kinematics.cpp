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
float calculate_norm(BLA::Matrix<3> v) {
    BLA::Matrix<1> inner = ~v * v;
    return sqrt(inner(0));
}

/// @brief Performs Newton's method for inverse kinematics. 
/// @param P_tgt Target point.
/// @param max_iter Max number of iterations allowed.
/// @param threshold Error allowed in X units.
/// @return Whether or not the movement was successful.
bool newton(BLA::Matrix<3> P_tgt, int max_iter, double threshold = 2.5f) {

    // --- algorithm ---
    // definitions:
    // X: double[] = [x y z]
    // Q: double[] = [m1, m2, ..., mn]
    // k: int
    // max_iter: int
    // threshold: double

    // init q
    // while k < max_iter
    //     E = X* - Xk
    //     if norm(E) < threshold EXIT
    //     dQk = J^-1 x E
    //     Qk+1 = Qk + dQk
    //     updateJacobian()
    // end while

    int k = 0;
    BLA::Matrix<3> E, P, Q, dQ;
    BLA::Matrix<3, 3> J;

    Q = temp_get_motor_angles();
    J = estimate_initial_jacobian();

    while (k < max_iter) {

        // calculate error
        P = forward_kinematics(Q);
        E = P_tgt - P;

        // check threshold
        if (calculate_norm(E) < threshold) {
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
        // temp_move_motors()
        // Q += dQ;

        // recompute the Jacobian
        // J = recompute_jacobian(Q);
                
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