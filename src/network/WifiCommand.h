#ifndef WIFI_COMMAND_H
#define WIFI_COMMAND_H
#include "WifiConfig.h"
#include "WifiCredentials.h"

enum class WifiCommandType : uint8_t {
    UNKNOWN,

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
    WifiConfig config;
    WifiCredentials credentials;
};

struct WifiCommand {
    WifiCommandType type = WifiCommandType::UNKNOWN;
    WifiCommandPayload payload{};
};
#endif
