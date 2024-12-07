#ifndef MOTORS_H
#define MOTORS_H

#include <BasicLinearAlgebra.h>
#include <Adafruit_PWMServoDriver.h>
#include <Encoder.h>

void moveMotor(Adafruit_PWMServoDriver &pwm, int motor_no, double degrees);
void moveMotors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles);

// TODO: get the degree value from the encoder! 
BLA::Matrix<3> getMotorAngles();

#endif