#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <Arduino.h>

class MotorDriver {
private:
    uint8_t _pwmPin;
    uint8_t _in1Pin;
    uint8_t _in2Pin;
    uint8_t _pwmChannel;
    uint8_t _minEffectivePwm;
    int16_t _output;
    uint8_t _scalePwm(uint8_t pwm) const;
    void _applyCoast();
    void _applyBrake();
    void _applyForward();
    void _applyReverse();
public:
    static constexpr uint8_t PWM_RESOLUTION = 8;
    static constexpr uint8_t MAX_OUTPUT = 255;
    MotorDriver(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t pwmChannel);
    void begin(uint32_t pwmFrequency = 1000, uint8_t minEffectivePwm = 0);
    void stop();
    void brake();
    bool isStopped() const;
    int16_t getOutput() const;
    void setOutput(int16_t output);
};
#endif
