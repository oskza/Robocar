#include <Arduino.h>
#include "nvs_flash.h"
#include "controllers/NetworkController.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED                   115200
#endif

NetworkStorage networkStorage;
Timer networkTimer;
NetworkController networkController(networkStorage, networkTimer);

void factoryReset() {
    nvs_flash_erase();
    nvs_flash_init();
    ESP.restart();
}

void setup() {
    // Serial.begin(MONITOR_SPEED);

    if (networkController.init())
        networkController.connect();
    else
        networkController.enableAP();
}

void loop() {
    if (networkController.tick()) {/** TODO: ... */}
}
