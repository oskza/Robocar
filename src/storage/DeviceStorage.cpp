#include "DeviceStorage.h"

void DeviceStorage::begin() { _prefs.begin("device_cfg", false); }

void DeviceStorage::reset() { _prefs.clear(); }

void DeviceStorage::loadConfig(DeviceConfig &target) { target.reportIntervalMs = _prefs.getULong("report_interval", DeviceDefaults::reportIntervalMs); }

void DeviceStorage::saveConfig(const DeviceConfig &cfg) { saveReportIntervalMs(cfg.reportIntervalMs); }

uint32_t DeviceStorage::loadReportIntervalMs() { return _prefs.getULong("report_interval", DeviceDefaults::reportIntervalMs); }

void DeviceStorage::saveReportIntervalMs(uint32_t ms) { _prefs.putULong("report_interval", ms); }
