#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H
#include <stdint.h>

struct NetworkConfig {
    bool dhcp;
    uint32_t intervalMs;
    char hostname[32];
    uint8_t localIP[4];
    uint8_t gateway[4];
    uint8_t subnet[4];
    uint8_t primaryDNS[4];
    uint8_t secondaryDNS[4];
};
#endif
