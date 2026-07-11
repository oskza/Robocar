#ifndef WIFI_STATION_STATE_H
#define WIFI_STATION_STATE_H
#include <stdint.h>

enum class WifiStationState : uint8_t {
    OFF,
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};
#endif
