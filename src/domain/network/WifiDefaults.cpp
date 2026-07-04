#include "WifiDefaults.h"
#include <cstring>

void WifiDefaults::applyConfig(WifiConfig &cfg) {
    strncpy(cfg.hostname, "Robocar", sizeof(cfg.hostname));
    cfg.hostname[sizeof(cfg.hostname) - 1] = '\0';
    cfg.dhcp = true;
    cfg.staticIp = {};
    cfg.reconnectIntervalMs = 5000;
}

void WifiDefaults::applyStationCredentials(WifiCredentials &credentials) {
    credentials.ssid[0] = '\0';
    credentials.password[0] = '\0';
}

void WifiDefaults::applyAccessPointCredentials(WifiCredentials &credentials) {
    strncpy(credentials.ssid, "Robocar", sizeof(credentials.ssid));
    credentials.ssid[sizeof(credentials.ssid) - 1] = '\0';

    strncpy(credentials.password, "12345678", sizeof(credentials.password));
    credentials.password[sizeof(credentials.password) - 1] = '\0';
}
