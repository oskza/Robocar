#ifndef WIFI_MODE_H
#define WIFI_MODE_H
#include <stdint.h>

enum class WifiMode : uint8_t {
    OFF, STA, AP, APSTA
};
#endif
