#include "MotionStorage.h"
#include "MotionDefaults.h"

namespace {
    constexpr const char *STORE_NAMESPACE = "motion";
    constexpr const char *KEY_CONFIG = "cfg";
}

MotionStorage::MotionStorage() : _store(STORE_NAMESPACE) {}

bool MotionStorage::loadConfig(MotionConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    cfg = MotionDefaults::config();
    return saveConfig(cfg);
}

bool MotionStorage::saveConfig(const MotionConfig &cfg) { return _store.save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool MotionStorage::clear() { return _store.clear(); }
