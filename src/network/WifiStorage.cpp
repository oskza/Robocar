#include "WifiStorage.h"
#include "WifiDefaults.h"

namespace {

constexpr const char *STORE_NAMESPACE = "wifi";
    constexpr const char *KEY_CONFIG = "cfg";
    constexpr const char *KEY_STATION_CREDENTIALS = "sta";
    constexpr const char *KEY_ACCESS_POINT_CREDENTIALS = "ap";
}

bool WifiStorage::begin() { return _store.begin(STORE_NAMESPACE); }

bool WifiStorage::loadConfig(WifiConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    WifiDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool WifiStorage::saveConfig(const WifiConfig &cfg) {
    return _store.save(KEY_CONFIG, &cfg, sizeof(cfg));
}

bool WifiStorage::resetConfig() {
    WifiConfig cfg{};
    WifiDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool WifiStorage::loadStationCredentials(WifiCredentials &credentials) {
    if (_store.load(
        KEY_STATION_CREDENTIALS,
        &credentials,
        sizeof(credentials)
    )) { return true; }
    WifiDefaults::applyStationCredentials(credentials);
    return saveStationCredentials(credentials);
}

bool WifiStorage::saveStationCredentials(const WifiCredentials &credentials) {
    return _store.save(
        KEY_STATION_CREDENTIALS,
        &credentials,
        sizeof(credentials)
    );
}

bool WifiStorage::resetStationCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyStationCredentials(credentials);
    return saveStationCredentials(credentials);
}

bool WifiStorage::loadAccessPointCredentials(WifiCredentials &credentials) {
    if (_store.load(
        KEY_ACCESS_POINT_CREDENTIALS,
        &credentials,
        sizeof(credentials)
    )) { return true; }
    WifiDefaults::applyAccessPointCredentials(credentials);
    return saveAccessPointCredentials(credentials);
}

bool WifiStorage::saveAccessPointCredentials(const WifiCredentials &credentials) {
    return _store.save(
        KEY_ACCESS_POINT_CREDENTIALS,
        &credentials,
        sizeof(credentials)
    );
}

bool WifiStorage::resetAccessPointCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyAccessPointCredentials(credentials);
    return saveAccessPointCredentials(credentials);
}

bool WifiStorage::resetAll() {
    if (!_store.clear())
        return false;
    bool ok = true;
    ok &= resetConfig();
    ok &= resetStationCredentials();
    ok &= resetAccessPointCredentials();
    return ok;
}
