#include "DeviceStorage.h"

void DeviceStorage::begin() { _prefs.begin("device_cfg", false); }

void DeviceStorage::reset() { _prefs.clear(); }

void DeviceStorage::loadConfig(DeviceConfig &target) { 
    target.statusReportIntervalMs = loadStatusReportIntervalMs();
}

void DeviceStorage::saveConfig(const DeviceConfig &cfg) { 
    saveStatusReportIntervalMs(cfg.statusReportIntervalMs);
}

uint32_t DeviceStorage::loadStatusReportIntervalMs() { 
    return _prefs.getULong("report_interval", DeviceDefaults::statusReportIntervalMs); 
}

void DeviceStorage::saveStatusReportIntervalMs(uint32_t ms) { _prefs.putULong("report_interval", ms); }
