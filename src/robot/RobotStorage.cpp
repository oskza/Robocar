#include "RobotStorage.h"
#include "RobotDefaults.h"

namespace {
    constexpr const char *STORE_NAMESPACE = "app";
    constexpr const char *KEY_CONFIG = "cfg";
}

bool RobotStorage::begin() { return _store.begin(STORE_NAMESPACE); }

bool RobotStorage::loadConfig(RobotConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    RobotDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool RobotStorage::saveConfig(const RobotConfig &cfg) {
    return _store.save(KEY_CONFIG, &cfg, sizeof(cfg));
}

bool RobotStorage::resetConfig() {
    RobotConfig cfg{};
    RobotDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
