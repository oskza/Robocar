#ifndef SYSTEM_STORAGE_H
#define SYSTEM_STORAGE_H
#include <Preferences.h>
#include "config/SystemConfig.h"

class SystemStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void clear();
    void loadConfig(SystemConfig &target);
    uint32_t loadStatusReportIntervalMs();
    void saveConfig(const SystemConfig &cfg);
    void saveStatusReportIntervalMs(uint32_t ms);
};
#endif
