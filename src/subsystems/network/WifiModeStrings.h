#ifndef WIFI_MODE_STRINGS_H
#define WIFI_MODE_STRINGS_H
#include "WifiMode.h"

class WifiModeStrings {
public:
    static const char *toString(WifiMode mode);
    static bool fromString(const char *text, WifiMode &mode);
};
#endif
