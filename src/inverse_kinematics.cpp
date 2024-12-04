// implement the lab 2 code
// specifically part 3ai; numerical (Newton's or Broyden's method)

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>

#include <vector>
#include <robot_arm.h>


void setup() {

}

void newton(std::vector<double> goal, int max_iter, double threshold) {
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
    while (k < max_iter) {
        // calculate error

        // check threshold

        // get dQ from J

        // move motors by dQ

        // recompute the jacobian using partial derivatives
    }


}


void loop() {

}