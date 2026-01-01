#include <Arduino.h>
#include <defaults.h>
#include "Robocar.h"

Timer driveTimer;
StopWatch driveStopwatch;
Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);
Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL, MOTOR_R_CORR, MOTOR_MIN_PWM);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR, MOTOR_MIN_PWM);
MotorController motorController(motorRight, motorLeft, encoderRight, encoderLeft, 
                                    driveTimer, driveStopwatch, WHEEL_DIAMETER, ENCODER_SLOTS);

Timer wifiTimer;
WifiController wifiController(&wifiTimer);

Timer wsTimer;
AsyncWebSocket ws(WEBSOCKET_PATH);
WebSocketController wsController(ws, wsTimer, WEBSOCKET_MAX_CLIENTS);

Timer joysticTimer;
AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(joystic, joysticTimer, JOYSTIC_DEADZONE);

CompassBMM150 compass;

Timer deviceTimer;
AsyncWebServer server(SERVER_PORT);
Robocar device(motorController, wifiController, wsController, 
                joysticController, compass, deviceTimer, server);

void setup() {
    // Serial.begin(115200);
    device.init(MOTOR_PWM_FREQ, MOTOR_PWM_RES, STATUS_REPORT_INTERVAL_MS, 
                    WIFI_INTERVAL_MS, WEBSOCKET_INTERVAL_MS, JOYSTIC_INTERVAL_MS);
}

void loop() { device.tick(); }
