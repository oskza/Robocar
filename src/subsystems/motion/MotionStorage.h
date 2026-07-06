#ifndef MOTION_STORAGE_H
#define MOTION_STORAGE_H
#include <PreferencesStore.h>
#include "MotionConfig.h"

class MotionStorage {
private:
    PreferencesStore _store;
public:
    bool begin();
    bool loadConfig(MotionConfig &cfg);
    bool saveConfig(const MotionConfig &cfg);
    bool resetConfig();
};
#endif
