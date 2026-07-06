#ifndef ROBOT_STORAGE_H
#define ROBOT_STORAGE_H
#include <PreferencesStore.h>
#include "RobotConfig.h"

class RobotStorage {
private:
    PreferencesStore _store;
public:
    bool begin();
    bool loadConfig(RobotConfig &cfg);
    bool saveConfig(const RobotConfig &cfg);
    bool resetConfig();
};
#endif
