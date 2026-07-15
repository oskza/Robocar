#ifndef WIFI_SNAPSHOT_H
#define WIFI_SNAPSHOT_H
#include <IPAddress.h>
#include "WifiConfig.h"
#include "WifiCredentials.h"
#include "WifiMode.h"
#include "WifiStationState.h"
#include "WifiAccessPointState.h"

struct WifiSnapshot {
    WifiMode mode;
    char hostname[WifiConfig::HOSTNAME_MAX_LEN + 1];
    struct WifiStationSnapshot {
        WifiStationState state;
        IPAddress ip;
        char ssid[WifiCredentials::SSID_MAX_LEN + 1];
        int8_t rssi;
        uint32_t reconnectAttempts;
    } station;
    struct WifiAccessPointSnapshot {
        WifiAccessPointState state;
        IPAddress ip;
        char ssid[WifiCredentials::SSID_MAX_LEN + 1];
        uint8_t clients;
    } accessPoint;
};
#endif
