#include <Arduino.h>
#include "domain/motion/MotionController.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED       115200
#endif

#define MOTOR_R_PWM_PIN     25
#define MOTOR_R_NORM_PIN    19
#define MOTOR_R_REV_PIN     18
#define MOTOR_R_PWM_CHANNEL 0

#define MOTOR_L_PWM_PIN     26
#define MOTOR_L_NORM_PIN    14
#define MOTOR_L_REV_PIN     27
#define MOTOR_L_PWM_CHANNEL 1

#define MOTOR_PWM_FREQ      1000
#define MOTOR_MIN_PWM       110
#define WHEEL_ACCELERATION  5

MotorDriver rightMotor(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
MotorDriver leftMotor(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

WheelOutputController rightWheel(rightMotor);
WheelOutputController leftWheel(leftMotor);

DifferentialDrive differential(rightWheel, leftWheel);
MotionController motion(differential);

void setup() {
    // Serial.begin(MONITOR_SPEED);

    rightMotor.begin(MOTOR_PWM_FREQ, MOTOR_MIN_PWM);
    leftMotor.begin(MOTOR_PWM_FREQ, MOTOR_MIN_PWM);

    motion.begin(WHEEL_ACCELERATION);

    motion.driveFor(150, 0, 3000);
}

void loop() {
    uint32_t now = millis();

    motion.update(now);
}
