#include "DeviceStorage.h"

void DeviceStorage::begin() { _prefs.begin("device_cfg", false); }

void DeviceStorage::reset() { _prefs.clear(); }

void DeviceStorage::loadConfig(DeviceConfig &target) { 
    target.statusReportIntervalMs = loadStatusReportIntervalMs();
    target.indicatorIntensity = loadIndicatorIntensity();
}

void DeviceStorage::saveConfig(const DeviceConfig &cfg) { 
    saveStatusReportIntervalMs(cfg.statusReportIntervalMs);
    saveIndicatorIntensity(cfg.indicatorIntensity);
}

uint32_t DeviceStorage::loadStatusReportIntervalMs() { 
    return _prefs.getULong("report_interval", DeviceDefaults::statusReportIntervalMs); 
}

void DeviceStorage::saveStatusReportIntervalMs(uint32_t ms) { _prefs.putULong("report_interval", ms); }

uint8_t DeviceStorage::loadIndicatorIntensity() {
    return _prefs.getUChar("indic_intens", DeviceDefaults::indicatorIntensity);
}

void DeviceStorage::saveIndicatorIntensity(uint8_t intensity) { _prefs.putUChar("indic_intens", intensity); }
