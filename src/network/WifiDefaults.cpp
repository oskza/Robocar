#include "WifiDefaults.h"
#include <string.h>

namespace WifiDefaults {
    WifiConfig config() {
        WifiConfig config{};
        config.mode = WifiMode::STA;
        strlcpy(config.hostname, "Robocar", sizeof(config.hostname));
        config.accessPointFallback = true;
        config.maxReconnectAttempts = 3;
        config.reconnectIntervalMs = 5000;
        config.stationConnectTimeoutMs = 15000;
        config.dhcp = true;
        config.staticIp = {};
        return config;
    }

    WifiCredentials stationCredentials() { return {}; }

    WifiCredentials accessPointCredentials() {
        WifiCredentials credentials{};
        strlcpy(credentials.ssid, "Robocar", sizeof(credentials.ssid));
        strlcpy(credentials.password, "12345678", sizeof(credentials.password));
        return credentials;
    }
}
