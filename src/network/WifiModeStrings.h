#ifndef WIFI_MODE_STRINGS_H
#define WIFI_MODE_STRINGS_H
#include "WifiMode.h"

namespace WifiModeStrings {
    const char *toString(WifiMode mode);
    bool fromString(const char *text, WifiMode &mode);
}
#endif
