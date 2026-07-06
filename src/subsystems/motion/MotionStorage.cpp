#include "MotionStorage.h"
#include "MotionDefaults.h"

static constexpr const char *NAMESPACE = "motion";
static constexpr const char *KEY_CONFIG = "cfg";

bool MotionStorage::begin() { return _store.begin(NAMESPACE); }

bool MotionStorage::loadConfig(MotionConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    MotionDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool MotionStorage::saveConfig(const MotionConfig &cfg) { return _store.save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool MotionStorage::resetConfig() {
    if (!_store.clear())
        return false;
    MotionConfig cfg{};
    MotionDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
