#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>
#include <StopWatch.h>
#include "../config/DriveConfig.h"
#include "../config/DriveConfigPatch.h"

enum class DriveMode : uint8_t { MANUAL, AUTO };

class DriveController {
private:
    Motor &_motorRight;
    Motor &_motorLeft;
    Encoder &_encoderRight;
    Encoder &_encoderLeft;
    Timer &_timer;
    StopWatch &_stopwatch;
    DriveMode _mode;
    float _circumference;
    uint8_t _encoderSlots;
    uint32_t _targetTicks;
    static DriveController *_instance;
    static void IRAM_ATTR _onRightEncoder();
    static void IRAM_ATTR _onLeftEncoder();
public:
    DriveController(Motor &motorRight, Motor &motorLeft, 
                        Encoder &encoderRight, Encoder &encoderLeft, 
                        Timer &timer, StopWatch &stopwatch);
    void init(const DriveConfig &cfg, DriveMode mode = DriveMode::AUTO);
    void stop();
    bool tick();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, 
                                bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, float meters);
    void setModeAuto();
    void setModeManual();
    bool isModeAuto() const;
    bool isModeManual() const;
    bool isDriving() const;
    uint32_t getDistanceTicks() const;
    float getDistanceMeters() const;
    uint32_t getDurationMs() const;
    static inline float wheelCircumference(float diameter, float correction = 1.00f) { 
        return diameter * PI * correction; 
    }
    static inline uint32_t metersToTicks(float meters, float circumference, uint8_t slots) { 
        return (uint32_t)round(meters / circumference * slots); 
    }
    static inline float ticksToMeters(uint32_t ticks, float circumference, uint8_t slots) { 
        return (ticks / (float)slots) * circumference; 
    }
};
#endif
