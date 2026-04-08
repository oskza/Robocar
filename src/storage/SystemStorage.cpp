#include "SystemStorage.h"
#include "defaults/SystemDefaults.h"

#define PREFS_SYSTEM_CFG            "sys_cfg"
#define PREFS_REPORT_INTERVAL_KEY   "report_interval"

void SystemStorage::begin() { _prefs.begin(PREFS_SYSTEM_CFG, false); }

void SystemStorage::clear() { _prefs.clear(); }

void SystemStorage::loadConfig(SystemConfig &target) { 
    target.statusReportIntervalMs = loadStatusReportIntervalMs();
}

uint32_t SystemStorage::loadStatusReportIntervalMs() { 
    return _prefs.getULong(PREFS_REPORT_INTERVAL_KEY, SystemDefaults::statusReportIntervalMs); 
}

void SystemStorage::saveConfig(const SystemConfig &cfg) { 
    saveStatusReportIntervalMs(cfg.statusReportIntervalMs);
}

void SystemStorage::saveStatusReportIntervalMs(uint32_t ms) { _prefs.putULong(PREFS_REPORT_INTERVAL_KEY, ms); }
