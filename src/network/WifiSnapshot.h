#ifndef WIFI_SNAPSHOT_H
#define WIFI_SNAPSHOT_H
#include "WifiConfig.h"
#include "WifiCredentials.h"
#include "WifiState.h"

struct WifiSnapshot {
    WifiMode mode = WifiMode::OFF;
    char hostname[WifiConfig::HOSTNAME_MAX_LEN + 1]{};
    struct WifiStationSnapshot {
        WifiStationState state = WifiStationState::OFF;
        IPAddress ip{};
        char ssid[WifiCredentials::SSID_MAX_LEN + 1]{};
        int8_t rssi = -127;
        uint32_t reconnectAttempts = 0;
    } station;
    struct WifiAccessPointSnapshot {
        WifiAccessPointState state = WifiAccessPointState::OFF;
        IPAddress ip{};
        char ssid[WifiCredentials::SSID_MAX_LEN + 1]{};
        uint8_t clients = 0;
    } accessPoint;
};
#endif
