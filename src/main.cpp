#include <Arduino.h>
#include <Bmm150Compass.h>
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

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

static constexpr uint32_t MOTOR_PWM_FREQ = 1000;
static constexpr uint8_t MOTOR_R_MIN_PWM = 110;
static constexpr uint8_t MOTOR_L_MIN_PWM = 110;

static constexpr uint8_t WHEEL_ACCELERATION = 5;

static constexpr float WHEEL_DIAMETER = 0.067f;
static constexpr float WHEEL_CIRCUMFERENCE_FACTOR = 1.0f;
static constexpr uint8_t ENCODER_SLOTS = 20;

static float wheelCircumference(float diameter, float factor = 1.0f) { return diameter * PI * factor; }

MotorDriver rightMotor(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
MotorDriver leftMotor(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

Encoder rightEncoder(ENCODER_R_PIN);
Encoder leftEncoder(ENCODER_L_PIN);

Bmm150Compass compass;

WheelOutputController rightWheel(rightMotor);
WheelOutputController leftWheel(leftMotor);

DifferentialDrive differential(rightWheel, leftWheel);
Odometry odometry(rightEncoder, leftEncoder);
MotionController motion(differential, odometry);

void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }

void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

void setup() {
    // Serial.begin(MONITOR_SPEED);

    rightMotor.begin(MOTOR_PWM_FREQ, MOTOR_R_MIN_PWM);
    leftMotor.begin(MOTOR_PWM_FREQ, MOTOR_L_MIN_PWM);

    rightEncoder.begin(onRightEncoder);
    leftEncoder.begin(onLeftEncoder);

    odometry.begin(wheelCircumference(WHEEL_DIAMETER, WHEEL_CIRCUMFERENCE_FACTOR), ENCODER_SLOTS);

    motion.begin(WHEEL_ACCELERATION);

    if (!compass.begin()) { /** TODO: handle init failure */}

    // motion.driveDistance(150, 0.50f);
}

void loop() {
    // motion.update(millis());
}
