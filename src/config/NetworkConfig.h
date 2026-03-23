#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H
#include <stdint.h>

struct NetworkConfig {
    char hostname[32];
    uint8_t localIP[4];
    uint8_t gateway[4];
    uint8_t subnet[4];
    uint8_t primaryDNS[4];
    uint8_t secondaryDNS[4];
    uint32_t intervalMs;
    char ssid[32];
    char password[64];
};
#endif
