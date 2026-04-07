#include "DeviceStorage.h"

#define PREFS_DEVICE_CFG            "device_cfg"
#define PREFS_REPORT_INTERVAL_KEY   "report_interval"

void DeviceStorage::begin() { _prefs.begin(PREFS_DEVICE_CFG, false); }

void DeviceStorage::reset() { _prefs.clear(); }

void DeviceStorage::loadConfig(DeviceConfig &target) { 
    target.statusReportIntervalMs = loadStatusReportIntervalMs();
}

uint32_t DeviceStorage::loadStatusReportIntervalMs() { 
    return _prefs.getULong(PREFS_REPORT_INTERVAL_KEY, DeviceDefaults::statusReportIntervalMs); 
}

void DeviceStorage::saveConfig(const DeviceConfig &cfg) { 
    saveStatusReportIntervalMs(cfg.statusReportIntervalMs);
}

void DeviceStorage::saveStatusReportIntervalMs(uint32_t ms) { _prefs.putULong(PREFS_REPORT_INTERVAL_KEY, ms); }
