#include <Arduino.h>
#include <Motor.h>

#define MOTOR_PWM_PIN       25 //ENA
#define MOTOR_FWD_PIN       18 //IN1
#define MOTOR_BCK_PIN       19 //IN2
#define MOTOR_PWM_CHANNEL   0

#define MOTORS_PWM_FREQ     1000
#define MOTORS_PWM_RES      8 //0-255

Motor motor(MOTOR_PWM_PIN, MOTOR_FWD_PIN, MOTOR_BCK_PIN, MOTOR_PWM_CHANNEL);

void setup() {
    motor.init(MOTORS_PWM_FREQ, MOTORS_PWM_RES);
}

void loop() {
    motor.forward(255);
    delay(3500);

    motor.stop();
    delay(1500);

    motor.backward(255);
    delay(3500);

    motor.stop();
    delay(1500);
}
