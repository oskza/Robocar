#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include <IPAddress.h>
#include "WifiMode.h"

struct WifiStaticIpConfig {
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDns;
    IPAddress secondaryDns;
};

struct WifiConfig {
    static constexpr uint8_t HOSTNAME_MAX_LEN = 32;
    WifiMode mode;
    char hostname[HOSTNAME_MAX_LEN + 1];
    bool dhcp;
    WifiStaticIpConfig staticIp;
    bool fallbackToAccessPoint;
    uint32_t reconnectIntervalMs;
};
#endif
