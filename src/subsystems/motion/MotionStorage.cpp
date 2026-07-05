#include "MotionStorage.h"
#include "MotionDefaults.h"

static constexpr const char *NAMESPACE = "motion";
static constexpr const char *KEY_CONFIG = "cfg";

bool MotionStorage::begin() { return _preferences.begin(NAMESPACE, false); }

bool MotionStorage::_load(const char *key, void *data, size_t size) {
    return _preferences.getBytes(key, data, size) == size;
}

bool MotionStorage::_save(const char *key, const void *data, size_t size) {
    return _preferences.putBytes(key, data, size) == size;
}

bool MotionStorage::loadConfig(MotionConfig &cfg) {
    if (_load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    MotionDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool MotionStorage::saveConfig(const MotionConfig &cfg) { return _save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool MotionStorage::resetConfig() {
    MotionConfig cfg{};
    MotionDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
