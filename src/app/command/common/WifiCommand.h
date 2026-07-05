#ifndef WIFI_COMMAND_H
#define WIFI_COMMAND_H
#include <stdint.h>
#include "../../../subsystems/network/WifiConfig.h"
#include "../../../subsystems/network/WifiCredentials.h"

enum class WifiCommand : uint8_t {
    STATUS,

    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,

    GET_STATION_CREDENTIALS,
    SET_STATION_CREDENTIALS,
    RESET_STATION_CREDENTIALS,

    GET_ACCESS_POINT_CREDENTIALS,
    SET_ACCESS_POINT_CREDENTIALS,
    RESET_ACCESS_POINT_CREDENTIALS,

    RESET_ALL
};

struct WifiCommandPayload {
    union {
        WifiConfig config;
        WifiCredentials credentials;
    };
};
#endif
