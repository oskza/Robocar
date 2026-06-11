#include <Arduino.h>
#include <Motor.h>
#include <Encoder.h>

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

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);

void setup() {
    Serial.begin(MONITOR_SPEED);
}

void loop() {}
