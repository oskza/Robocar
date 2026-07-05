#ifndef ROBOT_HARDWARE_CONFIG_H
#define ROBOT_HARDWARE_CONFIG_H
#include <stdint.h>

namespace RobotHardwareConfig {
    static constexpr uint8_t MOTOR_R_PWM_PIN = 25;
    static constexpr uint8_t MOTOR_R_NORM_PIN = 19;
    static constexpr uint8_t MOTOR_R_REV_PIN = 18;
    static constexpr uint8_t MOTOR_R_PWM_CHANNEL = 0;

    static constexpr uint8_t MOTOR_L_PWM_PIN = 26;
    static constexpr uint8_t MOTOR_L_NORM_PIN = 14;
    static constexpr uint8_t MOTOR_L_REV_PIN = 27;
    static constexpr uint8_t MOTOR_L_PWM_CHANNEL = 1;

    static constexpr uint32_t MOTOR_PWM_FREQ = 1000;

    static constexpr uint8_t ENCODER_R_PIN = 32;
    static constexpr uint8_t ENCODER_L_PIN = 33;
    static constexpr uint8_t ENCODER_SLOTS = 20;

    static constexpr uint8_t INA226_ADDRESS = 0x40;
    static constexpr float INA226_MAX_CURRENT_AMPS = 0.8f;
    static constexpr float INA226_SHUNT_OHMS = 0.1f;
}
#endif
