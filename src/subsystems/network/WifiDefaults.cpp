#include "WifiDefaults.h"
#include <cstring>

void WifiDefaults::applyConfig(WifiConfig &cfg) {
    cfg.mode = WifiMode::STA;
    strlcpy(cfg.hostname, "Robocar", sizeof(cfg.hostname));
    cfg.dhcp = true;
    cfg.staticIp = {};
    cfg.fallbackToAccessPoint = true;
    cfg.reconnectIntervalMs = 5000;
}

void WifiDefaults::applyStationCredentials(WifiCredentials &credentials) {
    credentials.ssid[0] = '\0';
    credentials.password[0] = '\0';
}

void WifiDefaults::applyAccessPointCredentials(WifiCredentials &credentials) {
    strlcpy(credentials.ssid, "Robocar", sizeof(credentials.ssid));
    strlcpy(credentials.password, "12345678", sizeof(credentials.password));
}
