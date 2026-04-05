#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H
#include <stdint.h>

struct WifiCredentials {
    char ssid[32];
    char password[64];
};
#endif
