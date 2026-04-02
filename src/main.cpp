#include <Arduino.h>
#include "nvs_flash.h"
#include "storage/DeviceStorage.h"
#include "controllers/DriveController.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED                   115200
#endif

#define MOTOR_R_PWM_CHANNEL             0
#define MOTOR_L_PWM_CHANNEL             1

#define MOTOR_R_PWM_PIN                 25
#define MOTOR_R_NORM_PIN                19
#define MOTOR_R_REV_PIN                 18

#define MOTOR_L_PWM_PIN                 26
#define MOTOR_L_NORM_PIN                14
#define MOTOR_L_REV_PIN                 27

#define ENCODER_R_PIN                   32
#define ENCODER_L_PIN                   33

Timer statusReportTimer;
DeviceStorage deviceStorage;

Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);

Timer driveTimer;
StopWatch driveStopwatch;

DriveStorage driveStorage;
DriveController driveController(motorRight, motorLeft, encoderRight, encoderLeft, 
                                    driveTimer, driveStopwatch, driveStorage);

void factoryReset() {
    nvs_flash_erase();
    nvs_flash_init();
    ESP.restart();
}

void getHeapStatus(JsonObject &target) {
    target["free"] = ESP.getFreeHeap();
    target["total"] = ESP.getHeapSize();
    target["maxAlloc"] = ESP.getMaxAllocHeap();
}

void createStatus(JsonDocument &doc) {
    doc["type"] = "status";
    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["uptime"] = millis();
    JsonObject heap = payload["heap"].to<JsonObject>();
    getHeapStatus(heap);
    JsonObject drive = payload["drive"].to<JsonObject>();
    driveController.getStatus(drive);
}

void setup() {
    // Serial.begin(MONITOR_SPEED);

    deviceStorage.begin();
    statusReportTimer.setTimeout(deviceStorage.loadStatusReportIntervalMs());
    statusReportTimer.start();

    driveController.init(DRIVE_MODE_AUTO);
}

void loop(void) {

    if (driveController.tick()) {/**...*/}

    if (statusReportTimer.tick()) {
        statusReportTimer.refresh();
        StaticJsonDocument<512> status;
        createStatus(status);
        //...
    }
}
