#ifndef ROBOT_STORAGE_H
#define ROBOT_STORAGE_H
#include <Preferences.h>
#include "RobotConfig.h"

class RobotStorage {
private:
    Preferences _preferences;
    bool _load(const char *key, void *data, size_t size);
    bool _save(const char *key, const void *data, size_t size);
public:
    bool begin();
    bool loadConfig(RobotConfig &cfg);
    bool saveConfig(const RobotConfig &cfg);
    bool resetConfig();
};
#endif
