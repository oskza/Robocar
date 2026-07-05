#ifndef MOTION_STORAGE_H
#define MOTION_STORAGE_H
#include <Preferences.h>
#include "MotionConfig.h"

class MotionStorage {
private:
    Preferences _preferences;
    bool _load(const char *key, void *data, size_t size);
    bool _save(const char *key, const void *data, size_t size);
public:
    bool begin();
    bool loadConfig(MotionConfig &cfg);
    bool saveConfig(const MotionConfig &cfg);
    bool resetConfig();
};
#endif
