#include "SystemController.h"
#include <Arduino.h>
#include <Esp.h>
#include <nvs_flash.h>

uint32_t SystemController::getUptimeMs() const { return millis(); }

SystemSnapshot SystemController::getSnapshot() const {
    SystemSnapshot snapshot{};
    snapshot.uptimeMs = getUptimeMs();
    snapshot.heap.freeBytes = ESP.getFreeHeap();
    snapshot.heap.minFreeBytes = ESP.getMinFreeHeap();
    snapshot.heap.maxAllocBytes = ESP.getMaxAllocHeap();
    return snapshot;
}

void SystemController::restart() { ESP.restart(); }

bool SystemController::factoryReset() {
    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK)
        return false;
    err = nvs_flash_init();
    if (err != ESP_OK)
        return false;
    ESP.restart();
    return true;
}
