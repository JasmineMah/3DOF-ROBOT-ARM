#include <motors.h>

void move_motor(Adafruit_PWMServoDriver &pwm, int motor_no, double degrees) {
    // TODO: move one individual motor by `degrees`
}

void move_motors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles) {
    for (int i = 0; i < delta_angles.Cols; i++) {
        move_motor(pwm, i, delta_angles(i));
    }
}

BLA::Matrix<3> get_motor_angles() {
    
}