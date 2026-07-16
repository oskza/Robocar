#ifndef WHEEL_ACTUATOR_H
#define WHEEL_ACTUATOR_H
#include <MotorDriver.h>

class WheelActuator {
private:
    static constexpr uint8_t DEFAULT_ACCELERATION = 5;
    MotorDriver &_motor;
    int16_t _targetOutput;
    int16_t _currentOutput;
    uint8_t _acceleration;
public:
    explicit WheelActuator(MotorDriver &motor);
    void begin(
        uint8_t acceleration,
        uint32_t pwmFrequency,
        uint8_t minEffectivePwm = 0
    );
    void update();
    void stop();
    void brake();
    bool isStopped() const;
    int16_t getCurrentOutput() const;
    int16_t getTargetOutput() const;
    void setTargetOutput(int16_t output);
    uint8_t getAcceleration() const;
    void setAcceleration(uint8_t acceleration);
};
#endif
