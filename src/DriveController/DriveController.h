#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <CompassBMM150.h>
#include <Timer.h>
#include <StopWatch.h>

#define DRIVE_MODE_MANUAL         0 
#define DRIVE_MODE_AUTO           1

#define AUTO_DRIVE_STATE_IDLE     0
#define AUTO_DRIVE_STATE_TIME     1
#define AUTO_DRIVE_STATE_DISTANCE 2
#define AUTO_DRIVE_STATE_ROTATION 3

class DriveController {
private:
    Motor &_motorRight;
    Motor &_motorLeft;
    Encoder &_encoderRight;
    Encoder &_encoderLeft;
    CompassBMM150 &_compass;
    Timer &_timer;
    StopWatch &_stopwatch;
    uint8_t _mode;
    uint8_t _autoState;
    uint8_t _slots;
    double _circumference;
    uint32_t _targetTicks;
    double _startHeading;
    double _targetHeading;
    static DriveController *_instance;
    static void IRAM_ATTR _onRightEncoder();
    static void IRAM_ATTR _onLeftEncoder();
public:
    DriveController(Motor &motorRight, Motor &motorLeft, 
                        Encoder &encoderRight, Encoder &encoderLeft, 
                        CompassBMM150 &compass, Timer &timer, StopWatch &stopwatch, 
                        uint8_t slots, double diameter);
    void init(uint32_t freq, uint8_t res);
    bool tick();
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, double meters);
    void rotate(int16_t turn, double angle);
    double getDistanceTicks() const;
    double getDistanceMeters() const;
    uint32_t getDurationMs() const;
    float getHeading() const;
    uint8_t getRightPWM() const;
    uint8_t getLeftPWM() const;
    const char* getAutoState() const;
    const char* getMode() const;
    void setMode(const char *mode);
    void setModeAuto();
    void setModeManual();
    bool isModeAuto() const;
    bool isModeManual() const;
    bool isDriving() const;
    static inline double wheelCircumference(double diameter) { return diameter * PI; }
    static inline uint32_t metersToTicks(double meters, double circumference, uint8_t slots) { 
        return (uint32_t)round(meters / circumference * slots); 
    }
    static inline double ticksToMeters(uint32_t ticks, double circumference, uint8_t slots) { 
        return (ticks / (double)slots) * circumference; 
    }
};
#endif
