#include "SystemController.h"
#include "defaults/SystemDefaults.h"

SystemController::SystemController(SystemStorage &storage, Timer &statusReportTimer)
                                    : _storage(storage), _statusReportTimer(statusReportTimer) {}

void SystemController::init() {
    _storage.begin();
    _statusReportTimer.setTimeout(_storage.loadStatusReportIntervalMs());
    _statusReportTimer.start();
}

/** TODO: ... */
bool SystemController::tick() {
    return _statusReportTimer.tick();
}

void SystemController::factoryReset() {
    nvs_flash_erase();
    nvs_flash_init();
    ESP.restart();
}

void SystemController::resetConfig() {
    _storage.clear();
    _statusReportTimer.setTimeout(SystemDefaults::statusReportIntervalMs);
}

void SystemController::restart() { ESP.restart(); }

void SystemController::getHeapStatus(HeapStatus &target) const {
    target.free = ESP.getFreeHeap();
    target.total = ESP.getHeapSize();
    target.maxAlloc = ESP.getMaxAllocHeap();
}

void SystemController::getStatus(SystemStatus &target) const {
    target.uptime = millis();
    getHeapStatus(target.heap);
}

void SystemController::getConfig(SystemConfig &target) const { _storage.loadConfig(target); }

void SystemController::updateConfig(const SystemConfig &cfg) {
    if (_statusReportTimer.getTimeout() == cfg.statusReportIntervalMs)
        return;
    _statusReportTimer.setTimeout(cfg.statusReportIntervalMs);
    _storage.saveConfig(cfg);
}
