#ifndef NETWORK_DEFAULTS_H
#define NETWORK_DEFAULTS_H
#include <stdint.h>

struct NetworkDefaults {
    static constexpr bool dhcp = true;
    static constexpr uint32_t intervalMs = 3000;
    static inline constexpr char hostname[32] = "robocar";
    static inline constexpr uint8_t localIP[4] = {192, 168, 1, 200};
    static inline constexpr uint8_t gateway[4] = {192, 168, 1, 1};
    static inline constexpr uint8_t subnet[4] = {255, 255, 255, 0};
    static inline constexpr uint8_t primaryDNS[4] = {8, 8, 8, 8};
    static inline constexpr uint8_t secondaryDNS[4] = {8, 8, 4, 4};
    static inline constexpr char apSSID[32] = "robocar";
    static inline constexpr char apPassword[64] = "12345678";
};
#endif
