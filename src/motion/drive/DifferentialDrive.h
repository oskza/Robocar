#ifndef DIFFERENTIAL_DRIVE_H
#define DIFFERENTIAL_DRIVE_H
#include "../wheel/WheelActuator.h"

class DifferentialDrive {
private:
    static constexpr int16_t MAX_OUTPUT = 255;
    WheelActuator &_leftWheel;
    WheelActuator &_rightWheel;
    static void _normalizeOutputs(int16_t &leftOutput, int16_t &rightOutput);
public:
    DifferentialDrive(WheelActuator &leftWheel, WheelActuator &rightWheel);
    void begin(
        uint8_t acceleration,
        uint32_t pwmFrequency,
        uint8_t leftMinEffectivePwm = 0,
        uint8_t rightMinEffectivePwm = 0
    );
    void update();
    void drive(int16_t velocity, int16_t turn);
    void stop();
    void brake();
    bool isStopped() const;
    int16_t getLeftOutput() const;
    int16_t getRightOutput() const;
};
#endif
