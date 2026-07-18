#include "WifiStorage.h"
#include "WifiDefaults.h"

namespace {
    constexpr const char *STORE_NAMESPACE = "wifi";
    constexpr const char *KEY_CONFIG = "cfg";
    constexpr const char *KEY_STATION_CREDENTIALS = "sta";
    constexpr const char *KEY_ACCESS_POINT_CREDENTIALS = "ap";
}

WifiStorage::WifiStorage() : _store(STORE_NAMESPACE) {}

bool WifiStorage::loadConfig(WifiConfig &config) {
    if (_store.load(KEY_CONFIG, &config, sizeof(config)))
        return true;
    config = WifiDefaults::config();
    return saveConfig(config);
}

bool WifiStorage::saveConfig(const WifiConfig &config) { return _store.save(KEY_CONFIG, &config, sizeof(config)); }

bool WifiStorage::loadStationCredentials(WifiCredentials &credentials) {
    if (_store.load(KEY_STATION_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    credentials = WifiDefaults::stationCredentials();
    return saveStationCredentials(credentials);
}

bool WifiStorage::saveStationCredentials(const WifiCredentials &credentials) {
    return _store.save(KEY_STATION_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::loadAccessPointCredentials(WifiCredentials &credentials) {
    if (_store.load(KEY_ACCESS_POINT_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    credentials = WifiDefaults::accessPointCredentials();
    return saveAccessPointCredentials(credentials);
}

bool WifiStorage::saveAccessPointCredentials(const WifiCredentials &credentials) {
    return _store.save(KEY_ACCESS_POINT_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::clear() { return _store.clear(); }
