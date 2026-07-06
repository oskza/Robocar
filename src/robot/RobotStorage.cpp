#include "RobotStorage.h"
#include "RobotDefaults.h"

static constexpr const char *NAMESPACE = "app";
static constexpr const char *KEY_CONFIG = "cfg";

bool RobotStorage::begin() { return _store.begin(NAMESPACE); }

bool RobotStorage::loadConfig(RobotConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    RobotDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool RobotStorage::saveConfig(const RobotConfig &cfg) { return _store.save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool RobotStorage::resetConfig() {
    if (!_store.clear())
        return false;
    RobotConfig cfg{};
    RobotDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
