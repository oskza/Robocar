#ifndef MOTION_STORAGE_H
#define MOTION_STORAGE_H
#include <PreferencesStore.h>
#include "MotionConfig.h"

class MotionStorage {
private:
    PreferencesStore _store;
public:
    MotionStorage();
    bool loadConfig(MotionConfig &cfg);
    bool saveConfig(const MotionConfig &cfg);
    bool clear();
};
#endif
