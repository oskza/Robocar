#ifndef WIFI_STATE_H
#define WIFI_STATE_H
#include <stdint.h>

enum class WifiState : uint8_t { OFF, DISCONNECTED, CONNECTING, CONNECTED, AP_ACTIVE };
#endif
