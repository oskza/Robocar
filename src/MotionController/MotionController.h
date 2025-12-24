#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>

#define ENCODER_SLOTS           20

#define WHEEL_DIAMETER          0.065
#define WHEEL_CIRCUMFERENCE     (WHEEL_DIAMETER * 3.14159265359)

#define METERS_TO_TICKS(meters) ((uint32_t)round((double)(meters) / WHEEL_CIRCUMFERENCE * ENCODER_SLOTS))
#define TICKS_TO_METERS(ticks)  (((double)(ticks) / ENCODER_SLOTS) * WHEEL_CIRCUMFERENCE)

#define MOTION_MODE_AUTO        0
#define MOTION_MODE_MANUAL      1

class MotionController {
private:
    Motor *_motorR;
    Motor *_motorL;
    Encoder *_encoderR;
    Encoder *_encoderL;
    Timer *_timer;
    uint8_t _mode;
    bool _isMoving;
    uint32_t _ticksToMove;
    void _move(int16_t velocity, int16_t turn);
public:
    MotionController(Motor *motorR, Motor *motorL, 
                        Encoder *encoderR, Encoder *encoderL, Timer *timer);
    static bool validateMode(uint8_t mode);
    void init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res);
    void tick();
    void stop();
    void moveManual(int16_t velocity, int16_t turn);
    void moveForMs(int16_t velocity, int16_t turn, uint32_t ms);
    void moveForMeters(int16_t velocity, int16_t turn, float meters);
    void moveForward(uint8_t velocity = MOTOR_MAX_PWM);
    void moveBackward(uint8_t velocity = MOTOR_MAX_PWM);
    void moveLeft(uint8_t velocity = MOTOR_MAX_PWM);
    void moveRight(uint8_t velocity = MOTOR_MAX_PWM);
    bool isMoving() const;
    bool isModeAuto() const;
    bool isModeManual() const;
    void setMode(uint8_t mode);
};
#endif
