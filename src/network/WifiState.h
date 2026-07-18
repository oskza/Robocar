#ifndef WIFI_STATE_H
#define WIFI_STATE_H

enum class WifiStationState : uint8_t {
    OFF,
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};

enum class WifiAccessPointState : uint8_t {
    OFF,
    ACTIVE
};

struct WifiState {
    WifiStationState station = WifiStationState::OFF;
    WifiAccessPointState accessPoint = WifiAccessPointState::OFF;
};
#endif
