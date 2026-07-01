#ifndef DIFFERENTIAL_DRIVE_H
#define DIFFERENTIAL_DRIVE_H
#include <Arduino.h>
#include "../wheel/WheelOutputController.h"

class DifferentialDrive {
private:
    WheelOutputController &_rightWheel;
    WheelOutputController &_leftWheel;
public:
    DifferentialDrive(WheelOutputController &rightWheel, WheelOutputController &leftWheel);
    void begin(uint8_t acceleration = 5);
    bool isStopped() const;
    int16_t getRightOutput() const;
    int16_t getLeftOutput() const;
    void drive(int16_t velocity, int16_t turn);
    void update();
    void stop();
    void brake();
};
#endif
