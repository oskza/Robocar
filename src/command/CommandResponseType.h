#ifndef COMMAND_RESPONSE_TYPE_H
#define COMMAND_RESPONSE_TYPE_H
#include <stdint.h>

enum class CommandResponseType : uint8_t {
    UNKNOWN,
    ACK,
    ERROR,
    ROBOT_STATUS,
    ROBOT_CONFIG,
    MOTION_STATUS,
    MOTION_CONFIG,
    WIFI_CONFIG,
    WIFI_CREDENTIALS
};
#endif
