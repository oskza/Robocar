#include "RobotStorage.h"
#include "RobotDefaults.h"

static constexpr const char *NAMESPACE = "app";
static constexpr const char *KEY_CONFIG = "cfg";

bool RobotStorage::_load(const char *key, void *data, size_t size) { return _preferences.getBytes(key, data, size) == size; }

bool RobotStorage::_save(const char *key, const void *data, size_t size) { return _preferences.putBytes(key, data, size) == size; }

bool RobotStorage::begin() { return _preferences.begin(NAMESPACE, false); }

bool RobotStorage::loadConfig(RobotConfig &cfg) {
    if (_load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    RobotDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool RobotStorage::saveConfig(const RobotConfig &cfg) { return _save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool RobotStorage::resetConfig() {
    if (!_preferences.clear())
        return false;
    RobotConfig cfg{};
    RobotDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}
