#ifndef MOTORS_H
#define MOTORS_H

#include <BasicLinearAlgebra.h>
#include <Adafruit_PWMServoDriver.h>
#include <Encoder.h>
#include <LiquidCrystal.h>

// Encoder definitions
#define ENCODER1_A 32
#define ENCODER1_B 4
#define ENCODER1_INDEX 13

#define ENCODER2_A 16
#define ENCODER2_B 17
#define ENCODER2_INDEX 18

#define ENCODER3_A 19
#define ENCODER3_B 21
#define ENCODER3_INDEX 15

// Servo pulse range
#define SERVOMIN 150 // minimum pulse length for homing
#define SERVOMAX 150 // maximum pulse length for slow motor movement

void moveMotor(Adafruit_PWMServoDriver &pwm, uint8_t motor_no, float degrees);
void moveMotors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles);
void homeMotor(Adafruit_PWMServoDriver &pwm, Encoder& encoder, int indexPin, int pwmChannel);
void grab(Adafruit_PWMServoDriver &pwm, bool grab);
BLA::Matrix<3> getMotorAngles();

// Encoder positions
extern long pos1, pos2, pos3;
extern Adafruit_PWMServoDriver pwm;
extern float base, elbow, wrist;


#endif