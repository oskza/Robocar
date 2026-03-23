#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>
#include <StopWatch.h>
#include "storage/DriveStorage.h"

#define DRIVE_MODE_MANUAL   0 
#define DRIVE_MODE_AUTO     1

class DriveController {
private:
    Motor &_motorRight;
    Motor &_motorLeft;
    Encoder &_encoderRight;
    Encoder &_encoderLeft;
    DriveStorage &_storage;
    Timer &_timer;
    StopWatch &_stopwatch;
    uint8_t _mode;
    DriveConfig _config;
    double _circumference;
    uint32_t _targetTicks;
    static DriveController *_instance;
    static void IRAM_ATTR _onRightEncoder();
    static void IRAM_ATTR _onLeftEncoder();
public:
    DriveController(Motor &motorRight, Motor &motorLeft, 
                        Encoder &encoderRight, Encoder &encoderLeft, 
                        DriveStorage &storage, Timer &timer, StopWatch &stopwatch);
    void init();
    bool tick();
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, 
                                bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, double meters);
    double getDistanceTicks() const;
    double getDistanceMeters() const;
    uint32_t getDurationMs() const;
    uint8_t getRightPWM() const;
    uint8_t getLeftPWM() const;
    const char* getMode() const;
    void setMode(const char *mode);
    void setModeAuto();
    void setModeManual();
    bool isModeAuto() const;
    bool isModeManual() const;
    bool isDriving() const;
    void getConfig(DriveConfig &target) const;
    void updateConfig(DriveConfig &cfg);
    void resetConfig();
    void updateFrequency(uint32_t freq);
    void updateResolution(uint8_t res);
    void updateWheelDiameter(double diameter);
    void updateEncoderSlots(uint8_t slots);
    void updateMotorRightMinPWM(uint8_t pwm);
    void updateMotorLeftMinPWM(uint8_t pwm);
    static inline double wheelCircumference(double diameter) { return diameter * PI; }
    static inline uint32_t metersToTicks(double meters, double circumference, uint8_t slots) { 
        return (uint32_t)round(meters / circumference * slots); 
    }
    static inline double ticksToMeters(uint32_t ticks, double circumference, uint8_t slots) { 
        return (ticks / (double)slots) * circumference; 
    }
};
#endif
