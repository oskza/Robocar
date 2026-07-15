#ifndef WIFI_STORAGE_H
#define WIFI_STORAGE_H
#include <PreferencesStore.h>
#include "WifiConfig.h"
#include "WifiCredentials.h"

class WifiStorage {
private:
    PreferencesStore _store;
public:
    bool begin();
    bool loadConfig(WifiConfig &cfg);
    bool saveConfig(const WifiConfig &cfg);
    bool resetConfig();
    bool loadStationCredentials(WifiCredentials &credentials);
    bool saveStationCredentials(const WifiCredentials &credentials);
    bool resetStationCredentials();
    bool loadAccessPointCredentials(WifiCredentials &credentials);
    bool saveAccessPointCredentials(const WifiCredentials &credentials);
    bool resetAccessPointCredentials();
    bool resetAll();
};
#endif
