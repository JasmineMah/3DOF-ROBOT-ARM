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

void newton(BLA::Matrix<3> Xt, int max_iter, double threshold) {
    // get your angles

    // --- algorithm ---
    // X: double[] = [x y z]
    // Q: double[] = [m1, m2, ..., mn]
    // k: int
    // max_iter: int
    // threshold: float

    // while k < max_iter
    // E = X* - Xk
    // if norm(E) < threshold EXIT
    // dQk = J^-1 x E
    // Qk+1 = Qk + dQk

    // might need to estimate initial Jacobian; move like you did with visual servoing and use forward kinematics instead.
    int k = 0;
    BLA::Matrix<3> E, X;
    BLA::Matrix<3, 3> J;
    std::vector<double> angles = {1.0f, 2.0f, 3.0f}; 

    estimateInitialJacobian();

    while (k < max_iter) {
        // get angles from the motor encoders

        // calculate error
        X = forwardKinematics(angles);
        E = Xt - X;

        // check threshold
        if (calculateNorm(E) < threshold) {
            break;
        }

        // get dQ from J


        // move motors by dQ


        // recompute the jacobian using partial derivatives
        
    }


}

float calculateNorm(BLA::Matrix<3> m) {
    BLA::Matrix<1> inner = ~m * m;
    return sqrt(inner(0));
}

void loop() {

}