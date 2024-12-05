#ifndef MOTORS_H
#define MOTORS_H

#include <BasicLinearAlgebra.h>
#include <Adafruit_PWMServoDriver.h>
#include <Encoder.h>

void move_motor(Adafruit_PWMServoDriver &pwm, int motor_no, double degrees);
void move_motors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles);

// TODO: get the degree value from the encoder! 
BLA::Matrix<3> get_motor_angles();

#endif