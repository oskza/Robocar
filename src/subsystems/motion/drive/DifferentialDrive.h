#ifndef DIFFERENTIAL_DRIVE_H
#define DIFFERENTIAL_DRIVE_H
#include <Arduino.h>
#include "../wheel/WheelOutputController.h"

class DifferentialDrive {
private:
    WheelOutputController &_leftWheel;
    WheelOutputController &_rightWheel;
public:
    DifferentialDrive(WheelOutputController &leftWheel, WheelOutputController &rightWheel);
    void begin(
        uint8_t acceleration,
        uint32_t pwmFrequency,
        uint8_t leftMinEffectivePwm = 0,
        uint8_t rightMinEffectivePwm = 0
    );
    bool isStopped() const;
    int16_t getRightOutput() const;
    int16_t getLeftOutput() const;
    void drive(int16_t velocity, int16_t turn);
    void update();
    void stop();
    void brake();
};
#endif
