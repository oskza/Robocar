#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>
#include <StopWatch.h>
#include <ArduinoJson.h>

#define ENCODER_SLOTS           20

#define WHEEL_DIAMETER          0.065
#define WHEEL_CIRCUMFERENCE     (WHEEL_DIAMETER * 3.14159265359)

#define METERS_TO_TICKS(meters) ((uint32_t)round((double)(meters) / WHEEL_CIRCUMFERENCE * ENCODER_SLOTS))
#define TICKS_TO_METERS(ticks)  (((double)(ticks) / ENCODER_SLOTS) * WHEEL_CIRCUMFERENCE)

#define DRIVE_MODE_AUTO        0
#define DRIVE_MODE_MANUAL      1

class DriveController {
private:
    Motor &_motorRight;
    Motor &_motorLeft;
    Encoder &_encoderRight;
    Encoder &_encoderLeft;
    Timer &_timer;
    StopWatch &_stopwatch;
    uint8_t _mode;
    uint32_t _targetTicks;
public:
    DriveController(Motor &motorRight, Motor &motorLeft, 
                        Encoder &encoderRight, Encoder &encoderLeft, 
                        Timer &timer, StopWatch &stopwatch);
    void init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res);
    void tick();
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, float meters);
    float getDistanceTicks() const;
    float getDistanceMeters() const;
    uint32_t getDurationMs() const;
    void setMode(const char *mode);
    void setModeAuto();
    void setModeManual();
    bool isModeAuto() const;
    bool isModeManual() const;
    bool isDriving() const;
    void getStatus(JsonObject &target) const;
};
#endif
