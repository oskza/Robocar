#ifndef WIFI_DEFAULTS_H
#define WIFI_DEFAULTS_H
#include "WifiConfig.h"
#include "WifiCredentials.h"

namespace WifiDefaults {
    WifiConfig config();
    WifiCredentials stationCredentials();
    WifiCredentials accessPointCredentials();
};
#endif
