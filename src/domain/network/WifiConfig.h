#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include <stdint.h>
#include <IPAddress.h>

struct WifiStaticIpConfig {
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDns;
    IPAddress secondaryDns;
};

struct WifiConfig {
    static constexpr uint8_t HOSTNAME_MAX_LEN = 32;
    char hostname[HOSTNAME_MAX_LEN + 1];
    bool dhcp;
    WifiStaticIpConfig staticIp;
    uint32_t reconnectIntervalMs;
};
#endif
