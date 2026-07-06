#include "WifiStorage.h"
#include "WifiDefaults.h"

static constexpr const char *NAMESPACE = "wifi";

static constexpr const char *KEY_CONFIG = "cfg";
static constexpr const char *KEY_STA_CREDENTIALS = "sta";
static constexpr const char *KEY_AP_CREDENTIALS = "ap";

bool WifiStorage::begin() { return _store.begin(NAMESPACE); }

bool WifiStorage::loadConfig(WifiConfig &cfg) {
    if (_store.load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    WifiDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool WifiStorage::saveConfig(const WifiConfig &cfg) { return _store.save(KEY_CONFIG, &cfg, sizeof(cfg)); }

bool WifiStorage::resetConfig() {
    WifiConfig cfg{};
    WifiDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool WifiStorage::loadStationCredentials(WifiCredentials &credentials) {
    if (_store.load(KEY_STA_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    WifiDefaults::applyStationCredentials(credentials);
    saveStationCredentials(credentials);
    return false;
}

bool WifiStorage::saveStationCredentials(const WifiCredentials &credentials) {
    return _store.save(KEY_STA_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::resetStationCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyStationCredentials(credentials);
    return saveStationCredentials(credentials);
}

bool WifiStorage::loadAccessPointCredentials(WifiCredentials &credentials) {
    if (_store.load(KEY_AP_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    WifiDefaults::applyAccessPointCredentials(credentials);
    saveAccessPointCredentials(credentials);
    return false;
}

bool WifiStorage::saveAccessPointCredentials(const WifiCredentials &credentials) {
    return _store.save(KEY_AP_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::resetAccessPointCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyAccessPointCredentials(credentials);
    return saveAccessPointCredentials(credentials);
}

bool WifiStorage::resetAll() {
    if (!_store.clear())
        return false;
    bool ok = resetConfig();
    ok &= resetStationCredentials();
    ok &= resetAccessPointCredentials();
    return ok;
}
