#ifndef WEBSOCKET_DEFAULTS_H
#define WEBSOCKET_DEFAULTS_H
#include <stdint.h>

struct WebSocketDefaults {
    static constexpr uint8_t maxClients = 2;
    static constexpr uint32_t intervalMs = 2000;
};
#endif
