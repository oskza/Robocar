#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>

#define ENCODER_SLOTS       20

#define WHEEL_DIAMETER      0.065
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * 3.14159265359)

#define METERS_TO_TICKS(meters) ((uint32_t)round((double)(meters) / WHEEL_CIRCUMFERENCE * ENCODER_SLOTS))
#define TICKS_TO_METERS(ticks) (((double)(ticks) / ENCODER_SLOTS) * WHEEL_CIRCUMFERENCE)

class MotionController {
private:
    Motor *_motorR;
    Motor *_motorL;
    Encoder *_encoderR;
    Encoder *_encoderL;
    Timer *_timer;
public:
    MotionController(Motor *motorR, Motor *motorL, 
                        Encoder *encoderR, Encoder *encoderL, Timer *timer);
    void init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res);
    void stop();
    void move(int16_t velocity, int16_t turn);
    bool moveForMs(int16_t velocity, int16_t turn, uint32_t ms);
    bool moveForMeters(int16_t velocity, int16_t turn, float meters);
};
#endif
