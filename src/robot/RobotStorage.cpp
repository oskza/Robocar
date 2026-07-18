#include "RobotStorage.h"
#include "RobotDefaults.h"

namespace {
    constexpr char STORE_NAMESPACE[] = "app";
    constexpr char KEY_CONFIG[] = "cfg";
}

RobotStorage::RobotStorage() : _store(STORE_NAMESPACE) {}

bool RobotStorage::loadConfig(RobotConfig &config) {
    if (_store.load(KEY_CONFIG, &config, sizeof(config)))
        return true;
    config = RobotDefaults::config();
    return saveConfig(config);
}

bool RobotStorage::saveConfig(const RobotConfig &config) {
    return _store.save(KEY_CONFIG, &config, sizeof(config));
}

bool RobotStorage::clear() { return _store.clear(); }
