#ifndef DEVICE_STORAGE_H
#define DEVICE_STORAGE_H
#include <Preferences.h>
#include "config/DeviceConfig.h"
#include "defaults/DeviceDefaults.h"

class DeviceStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(DeviceConfig &target);
    void saveConfig(const DeviceConfig &cfg);
    uint32_t loadStatusReportIntervalMs();
    void saveStatusReportIntervalMs(uint32_t ms);
};
#endif
