#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include <IPAddress.h>
#include "WifiMode.h"

struct WifiStaticIpConfig {
    IPAddress ip{};
    IPAddress gateway{};
    IPAddress subnet{};
    IPAddress primaryDns{};
    IPAddress secondaryDns{};
};

struct WifiConfig {
    static constexpr uint8_t HOSTNAME_MAX_LEN = 32;
    WifiMode mode = WifiMode::OFF;
    char hostname[HOSTNAME_MAX_LEN + 1]{};
    bool accessPointFallback = false;
    uint32_t maxReconnectAttempts = 0;
    uint32_t reconnectIntervalMs = 0;
    uint32_t stationConnectTimeoutMs = 0;
    bool dhcp = false;
    WifiStaticIpConfig staticIp{};
};
#endif
