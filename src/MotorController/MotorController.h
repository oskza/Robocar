#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H
#include <Motor.h>
#include <Encoder.h>
#include <Timer.h>
#include <StopWatch.h>
#include <ArduinoJson.h>

#define DRIVE_MODE_AUTO     0
#define DRIVE_MODE_MANUAL   1

class MotorController {
private:
    Motor &_motorRight;
    Motor &_motorLeft;
    Encoder &_encoderRight;
    Encoder &_encoderLeft;
    Timer &_timer;
    StopWatch &_stopwatch;
    double _diameter;
    uint8_t _slots;
    uint8_t _mode;
    uint32_t _targetTicks;
    static MotorController *_instance;
    static void IRAM_ATTR _onRightEncoder();
    static void IRAM_ATTR _onLeftEncoder();
public:
    MotorController(Motor &motorRight, Motor &motorLeft, 
                        Encoder &encoderRight, Encoder &encoderLeft, 
                        Timer &timer, StopWatch &stopwatch, double diameter, uint8_t slots);
    static inline double wheelCircumference(double diameter);
    static inline uint32_t metersToTicks(double meters, double circumference, uint8_t slots);
    static inline double ticksToMeters(uint32_t ticks, double circumference, uint8_t slots);
    void init(uint32_t freq, uint8_t res);
    void tick();
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, float meters);
    double getDistanceTicks() const;
    double getDistanceMeters() const;
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
