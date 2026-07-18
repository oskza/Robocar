#ifndef WIFI_STORAGE_H
#define WIFI_STORAGE_H
#include <PreferencesStore.h>
#include "WifiConfig.h"
#include "WifiCredentials.h"

class WifiStorage {
private:
    PreferencesStore _store;
public:
    WifiStorage();
    bool loadConfig(WifiConfig &config);
    bool saveConfig(const WifiConfig &config);
    bool loadStationCredentials(WifiCredentials &credentials);
    bool saveStationCredentials(const WifiCredentials &credentials);
    bool loadAccessPointCredentials(WifiCredentials &credentials);
    bool saveAccessPointCredentials(const WifiCredentials &credentials);
    bool clear();
};
#endif
