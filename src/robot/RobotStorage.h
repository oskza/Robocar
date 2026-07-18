#ifndef ROBOT_STORAGE_H
#define ROBOT_STORAGE_H
#include <PreferencesStore.h>
#include "RobotConfig.h"

class RobotStorage {
private:
    PreferencesStore _store;
public:
    RobotStorage();
    bool loadConfig(RobotConfig &config);
    bool saveConfig(const RobotConfig &config);
    bool clear();
};
#endif
