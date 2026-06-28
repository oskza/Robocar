#ifndef WHEEL_OUTPUT_CONTROLLER_H
#define WHEEL_OUTPUT_CONTROLLER_H
#include <MotorDriver.h>

class WheelOutputController {
private:
    MotorDriver &_motor;
    int16_t _targetOutput;
    int16_t _currentOutput;
    uint8_t _acceleration;
public:
    WheelOutputController(MotorDriver &motor);
    void begin(uint8_t acceleration = 5);
    bool isStopped() const;
    uint8_t getAcceleration() const;
    void setAcceleration(uint8_t acceleration);
    int16_t getCurrentOutput() const;
    int16_t getTargetOutput() const;
    void setTargetOutput(int16_t output);
    void update();
    void stop();
    void brake();
};
#endif
