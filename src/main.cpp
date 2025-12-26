#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <CompassBMM150.h>
#include "DriveController/DriveController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include "WifiController/WifiController.h"
#include <wifi_creds.h>

#define MOTOR_R_PWM_PIN         25
#define MOTOR_R_FWD_PIN         18
#define MOTOR_R_BCK_PIN         19

#define MOTOR_L_PWM_PIN         26
#define MOTOR_L_FWD_PIN         27
#define MOTOR_L_BCK_PIN         14

#define MOTOR_R_PWM_CHANNEL     0
#define MOTOR_L_PWM_CHANNEL     1

#define MOTOR_PWM_FREQ          1000
#define MOTOR_PWM_RES           8

#define MOTOR_R_CORR            1
#define MOTOR_L_CORR            1.0395f

#define ENCODER_R_PIN           32
#define ENCODER_L_PIN           33

#define JOYSTIC_VERT_PIN        34
#define JOYSTIC_HORZ_PIN        35
#define JOYSTIC_DEADZONE        112
#define JOYSTIC_INTERVAL_MS     20

Timer timerDrive;
Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);
Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL, MOTOR_R_CORR);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
DriveController driveController(motorRight, motorLeft, encoderRight, encoderLeft, timerDrive);

Timer timerJoystic;
AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(joystic, timerJoystic, JOYSTIC_DEADZONE);

void IRAM_ATTR onRightEncoder() { encoderRight.tick(); }
void IRAM_ATTR onLeftEncoder() { encoderLeft.tick(); }

void setup() {
    // Serial.begin(115200);

    driveController.init(onRightEncoder, onLeftEncoder, MOTOR_PWM_FREQ, MOTOR_PWM_RES);

    // joysticController.init(JOYSTIC_INTERVAL_MS);

    driveController.driveDistance(150, 1.2);
}

void loop() {
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)
            && (vert != 0 || horz != 0 || driveController.isDriving()))
        driveController.driveDifferential(vert, horz);

    driveController.tick();
}
