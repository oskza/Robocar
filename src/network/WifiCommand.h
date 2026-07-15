#ifndef WIFI_COMMAND_H
#define WIFI_COMMAND_H
#include <stdint.h>
#include "network/WifiConfig.h"
#include "network/WifiCredentials.h"

enum class WifiCommand : uint8_t {
    STATUS,

    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,

    SET_STATION_CREDENTIALS,
    RESET_STATION_CREDENTIALS,

    GET_ACCESS_POINT_CREDENTIALS,
    SET_ACCESS_POINT_CREDENTIALS,
    RESET_ACCESS_POINT_CREDENTIALS,

    RESET_ALL
};

struct WifiCommandPayload {
    WifiConfig cfg;
    WifiCredentials credentials;
};
#endif
