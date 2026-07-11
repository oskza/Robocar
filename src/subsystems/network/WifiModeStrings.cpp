#include "WifiModeStrings.h"
#include <string.h>

const char *WifiModeStrings::toString(WifiMode mode) {
    switch (mode) {
        case WifiMode::OFF: return "off";
        case WifiMode::STA: return "sta";
        case WifiMode::AP: return "ap";
        case WifiMode::APSTA: return "apsta";
    }
    return "unknown";
}

bool WifiModeStrings::fromString(const char *text, WifiMode &mode) {
    if (!text)
        return false;
    if (strcmp(text, "off") == 0) {
        mode = WifiMode::OFF;
        return true;
    }
    if (strcmp(text, "sta") == 0) {
        mode = WifiMode::STA;
        return true;
    }
    if (strcmp(text, "ap") == 0) {
        mode = WifiMode::AP;
        return true;
    }
    if (strcmp(text, "apsta") == 0) {
        mode = WifiMode::APSTA;
        return true;
    }
    return false;
}
