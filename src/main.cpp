#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <CompassBMM150.h>
#include "DriveController/DriveController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include "WifiController/WifiController.h"
#include <wifi_creds.h>

#define JOYSTIC_VERT_PIN        34
#define JOYSTIC_HORZ_PIN        35
#define JOYSTIC_DEADZONE        112
#define JOYSTIC_INTERVAL_MS     20

Timer timerJoystic;

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(joystic, timerJoystic, JOYSTIC_DEADZONE);

void setup() {
    // Serial.begin(115200);

    joysticController.init(JOYSTIC_INTERVAL_MS);
}

void loop() {
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)) {/**/}
}
