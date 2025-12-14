#include <Arduino.h>
#include <MotorsX2.h>
#include <Encoder.h>
#include <CompassBMM150.h>
#include <AnalogJoystic.h>
#include <Timer.h>

#define WHEEL_DIAMETER      0.065
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * 3.14159265359)

#define MOTOR_R_PWM_PIN     25
#define MOTOR_R_FWD_PIN     18
#define MOTOR_R_BCK_PIN     19
#define MOTOR_R_PWM_CHANNEL 0

#define MOTOR_L_PWM_PIN     26
#define MOTOR_L_FWD_PIN     27
#define MOTOR_L_BCK_PIN     14
#define MOTOR_L_PWM_CHANNEL 1

#define MOTOR_L_CORR        1.0395f

#define MOTORS_PWM_FREQ     1000
#define MOTORS_PWM_RES      8

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

#define ENCODER_SLOTS       20

#define JOYSTIC_HORZ_PIN    35
#define JOYSTIC_VERT_PIN    34
#define JOYSTIC_DEADZONE    7

#define METERS_TO_TICKS(meters) ((uint32_t)round((double)(meters) / WHEEL_CIRCUMFERENCE * ENCODER_SLOTS))
#define TICKS_TO_METERS(ticks) (((double)(ticks) / ENCODER_SLOTS) * WHEEL_CIRCUMFERENCE)

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
MotorsX2 motors(&motorR, &motorL);

Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);

CompassBMM150 compass;

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN, JOYSTIC_DEADZONE);

Timer timer;

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

void joysticHandle() {
    int16_t vert = joystic.readVerticalPWM();
    int16_t horz = joystic.readHorizontalPWM();
    if(joystic.isInDeadzone(vert, horz)) {
        motors.stop();
        return;
    }
    motors.move(vert, horz);
}

bool moveForMs(int16_t velocity, int16_t turn, uint32_t ms) {
    if(timer.tick()) {
        motors.stop();
        timer.reset();
        return true;
    }
    if(!timer.isRunning()) {
        timer.setTimeout(ms);
        timer.start();
    }
    motors.move(velocity, turn);
    return false;
}

bool moveForMeters(int16_t velocity, int16_t turn, float meters) {
    uint32_t ticks = METERS_TO_TICKS(meters);
    if(encoderR.getCount() >= ticks 
            || encoderL.getCount() >= ticks) {
        motors.stop();
        encoderR.reset();
        encoderL.reset();
        return true;
    }
    motors.move(velocity, turn);
    return false;
}

void setup() {
    Serial.begin(115200);

    motors.init(MOTORS_PWM_FREQ, MOTORS_PWM_RES);

    // encoderR.init(onRightEncoder);
    // encoderL.init(onLeftEncoder);

    // if (!compass.init()) {/*...*/}

    joystic.calibrateCenter();
}

void loop() {
    joysticHandle();
}
