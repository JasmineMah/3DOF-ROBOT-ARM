#include <motors.h>

void moveMotor(Adafruit_PWMServoDriver &pwm, int motor_no, double degrees) {
    // TODO: move one individual motor by `degrees`
}

void moveMotors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles) {
    for (int i = 0; i < delta_angles.Cols; i++) {
        moveMotor(pwm, i, delta_angles(i));
    }
}

BLA::Matrix<3> getMotorAngles() {
    
}