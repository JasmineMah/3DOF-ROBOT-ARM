#ifndef MOTORS
#define MOTORS

#include <BasicLinearAlgebra.h>
#include <Adafruit_PWMServoDriver.h>


// move a specific motor
void move_motor(Adafruit_PWMServoDriver &pwm, int motor_no) {
    
}

void move_motors(BLA::Matrix<3> angles, Adafruit_PWMServoDriver &pwm) {
    for (int i = 0; i < angles.Cols; i++) {
        move_motor(pwm, i);
    }
}

#endif