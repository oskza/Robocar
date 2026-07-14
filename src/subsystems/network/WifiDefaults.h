#ifndef WIFI_DEFAULTS_H
#define WIFI_DEFAULTS_H
#include "WifiConfig.h"
#include "WifiCredentials.h"

class WifiDefaults {
public:
    static void applyConfig(WifiConfig &cfg);
    static void applyStationCredentials(WifiCredentials &credentials);
    static void applyAccessPointCredentials(WifiCredentials &credentials);
};
#endif
