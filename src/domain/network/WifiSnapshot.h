#ifndef WIFI_SNAPSHOT_H
#define WIFI_SNAPSHOT_H
#include <stdint.h>
#include <IPAddress.h>
#include "WifiConfig.h"
#include "WifiCredentials.h"
#include "WifiMode.h"
#include "WifiState.h"

struct WifiSnapshot {
    WifiMode mode;
    WifiState state;
    bool connected;
    bool accessPointActive;
    int8_t rssi;
    IPAddress localIp;
    IPAddress accessPointIp;
    char ssid[WifiCredentials::SSID_MAX_LEN + 1];
    char hostname[WifiConfig::HOSTNAME_MAX_LEN + 1];
    uint32_t reconnectAttempts;
};
#endif
