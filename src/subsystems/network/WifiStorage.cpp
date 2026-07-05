#include "WifiStorage.h"
#include "WifiDefaults.h"

static constexpr const char *NAMESPACE = "wifi";

static constexpr const char *KEY_CONFIG = "cfg";
static constexpr const char *KEY_STA_CREDENTIALS = "sta";
static constexpr const char *KEY_AP_CREDENTIALS = "ap";

bool WifiStorage::begin() { return _preferences.begin(NAMESPACE, false); }

bool WifiStorage::_load(const char *key, void *data, size_t size) {
    return _preferences.getBytes(key, data, size) == size;
}

bool WifiStorage::_save(const char *key, const void *data, size_t size) {
    return _preferences.putBytes(key, data, size) == size;
}

bool WifiStorage::loadConfig(WifiConfig &cfg) {
    if (_load(KEY_CONFIG, &cfg, sizeof(cfg)))
        return true;
    WifiDefaults::applyConfig(cfg);
    saveConfig(cfg);
    return false;
}

bool WifiStorage::saveConfig(const WifiConfig &cfg) {
    return _save(KEY_CONFIG, &cfg, sizeof(cfg));
}

bool WifiStorage::resetConfig() {
    WifiConfig cfg{};
    WifiDefaults::applyConfig(cfg);
    return saveConfig(cfg);
}

bool WifiStorage::loadStationCredentials(WifiCredentials &credentials) {
    if (_load(KEY_STA_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    WifiDefaults::applyStationCredentials(credentials);
    saveStationCredentials(credentials);
    return false;
}

bool WifiStorage::saveStationCredentials(const WifiCredentials &credentials) {
    return _save(KEY_STA_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::resetStationCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyStationCredentials(credentials);
    return saveStationCredentials(credentials);
}

bool WifiStorage::loadAccessPointCredentials(WifiCredentials &credentials) {
    if (_load(KEY_AP_CREDENTIALS, &credentials, sizeof(credentials)))
        return true;
    WifiDefaults::applyAccessPointCredentials(credentials);
    saveAccessPointCredentials(credentials);
    return false;
}

bool WifiStorage::saveAccessPointCredentials(const WifiCredentials &credentials) {
    return _save(KEY_AP_CREDENTIALS, &credentials, sizeof(credentials));
}

bool WifiStorage::resetAccessPointCredentials() {
    WifiCredentials credentials{};
    WifiDefaults::applyAccessPointCredentials(credentials);
    return saveAccessPointCredentials(credentials);
}

bool WifiStorage::resetAll() {
    if (!_preferences.clear())
        return false;
    bool ok = resetConfig();
    ok &= resetStationCredentials();
    ok &= resetAccessPointCredentials();
    return ok;
}
