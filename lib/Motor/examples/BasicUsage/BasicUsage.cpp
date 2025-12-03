#include <Arduino.h>
#include <MotorsX2.h>

#define MOTOR_R_PWM_PIN     25 //ENA
#define MOTOR_R_FWD_PIN     18 //IN1
#define MOTOR_R_BCK_PIN     19 //IN2
#define MOTOR_R_PWM_CHANNEL 0

#define MOTOR_L_PWM_PIN     26 //ENB
#define MOTOR_L_FWD_PIN     27 //IN4
#define MOTOR_L_BCK_PIN     14 //IN3
#define MOTOR_L_PWM_CHANNEL 1

#define MOTORS_PWM_FREQ     1000
#define MOTORS_PWM_RES      8

#define MOTORS_MIN_PWM      110

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

#define ENCODER_SLOTS       20

#define JOYSTIC_HORZ_PIN    35 //X
#define JOYSTIC_VERT_PIN    34 //Y
#define JOYSTIC_DEADZONE    80

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL);
MotorsX2 motors(&motorR, &motorL);

void setup() {
    motors.init(MOTORS_PWM_FREQ, MOTORS_PWM_RES);
}

void loop() {
    motors.moveForward(255);
    delay(3500);

    motors.stop();
    delay(1500);

    motors.moveBackward(255);
    delay(3500);

    motors.stop();
    delay(1500);
}
