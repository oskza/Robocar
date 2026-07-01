#include <Arduino.h>
#include <Bmm150Compass.h>
#include "domain/motion/MotionController.h"

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
static constexpr float HEADING_TOLERANCE_DEGREES = 12.0f;
static constexpr uint8_t ROTATION_SPEED = 35;

static constexpr uint32_t MOTION_UPDATE_INTERVAL_MS = 100;
static constexpr uint32_t SETTLE_DELAY_MS = 1000;

static constexpr float WHEEL_DIAMETER = 0.067f;
static constexpr float WHEEL_CIRCUMFERENCE_FACTOR = 1.0f;
static constexpr uint8_t ENCODER_SLOTS = 20;

static float wheelCircumference(float diameter, float factor = 1.0f) {
    return diameter * PI * factor;
}

MotorDriver rightMotor(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
MotorDriver leftMotor(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

Encoder rightEncoder(ENCODER_R_PIN);
Encoder leftEncoder(ENCODER_L_PIN);

Bmm150Compass compass;

WheelOutputController rightWheel(rightMotor);
WheelOutputController leftWheel(leftMotor);

DifferentialDrive differential(rightWheel, leftWheel);
Odometry odometry(rightEncoder, leftEncoder);
MotionController motion(differential, odometry, compass);

void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }
void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

enum class TestStep : uint8_t {
    WAIT_ROTATE_LEFT,
    SETTLE_AFTER_LEFT,
    WAIT_ROTATE_RIGHT,
    SETTLE_AFTER_RIGHT,
    WAIT_ROTATE_NORTH,
    DONE
};

TestStep step = TestStep::WAIT_ROTATE_LEFT;

uint32_t lastMotionUpdateMs = 0;
uint32_t settleStartMs = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);

    rightMotor.begin(MOTOR_PWM_FREQ, MOTOR_R_MIN_PWM);
    leftMotor.begin(MOTOR_PWM_FREQ, MOTOR_L_MIN_PWM);

    rightEncoder.begin(onRightEncoder);
    leftEncoder.begin(onLeftEncoder);

    odometry.begin(
        wheelCircumference(WHEEL_DIAMETER, WHEEL_CIRCUMFERENCE_FACTOR),
        ENCODER_SLOTS
    );

    compass.begin();

    motion.begin(WHEEL_ACCELERATION, HEADING_TOLERANCE_DEGREES);

    motion.rotateBy(-90.0f, ROTATION_SPEED);
}

void loop() {
    uint32_t now = millis();

    if (now - lastMotionUpdateMs >= MOTION_UPDATE_INTERVAL_MS) {
        lastMotionUpdateMs = now;
        motion.update(now);
    }

    switch (step) {
        case TestStep::WAIT_ROTATE_LEFT:
            if (motion.isStopped()) {
                settleStartMs = now;
                step = TestStep::SETTLE_AFTER_LEFT;
            }
            break;

        case TestStep::SETTLE_AFTER_LEFT:
            if (now - settleStartMs >= SETTLE_DELAY_MS) {
                motion.rotateBy(90.0f, ROTATION_SPEED);
                step = TestStep::WAIT_ROTATE_RIGHT;
            }
            break;

        case TestStep::WAIT_ROTATE_RIGHT:
            if (motion.isStopped()) {
                settleStartMs = now;
                step = TestStep::SETTLE_AFTER_RIGHT;
            }
            break;

        case TestStep::SETTLE_AFTER_RIGHT:
            if (now - settleStartMs >= SETTLE_DELAY_MS) {
                motion.rotateTo(0.0f, ROTATION_SPEED);
                step = TestStep::WAIT_ROTATE_NORTH;
            }
            break;

        case TestStep::WAIT_ROTATE_NORTH:
            if (motion.isStopped()) {
                step = TestStep::DONE;
            }
            break;

        case TestStep::DONE:
            break;
    }
}
