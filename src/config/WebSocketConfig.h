#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H
#include <stdint.h>

struct WebSocketConfig {
    uint8_t maxClients;
    uint32_t intervalMs;
};
#endif
