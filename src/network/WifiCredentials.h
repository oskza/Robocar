#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H
#include <stdint.h>

struct WifiCredentials {
    static constexpr uint8_t SSID_MAX_LEN = 32;
    static constexpr uint8_t PASS_MAX_LEN = 64;
    char ssid[SSID_MAX_LEN + 1];
    char password[PASS_MAX_LEN + 1];
};
#endif
