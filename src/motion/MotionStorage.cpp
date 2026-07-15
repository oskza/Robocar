#include "MotionStorage.h"
#include "MotionDefaults.h"

namespace {
    constexpr const char *STORE_NAMESPACE = "motion";
    constexpr const char *KEY_CONFIG = "cfg";
}

bool MotionStorage::begin() { return _store.begin(STORE_NAMESPACE); }

bool MotionStorage::loadConfig(MotionConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    MotionDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool MotionStorage::saveConfig(const MotionConfig &cfg) {
    return _store.save(KEY_CONFIG, &cfg, sizeof(cfg));
}

bool MotionStorage::resetConfig() {
    MotionConfig cfg{};
    MotionDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
