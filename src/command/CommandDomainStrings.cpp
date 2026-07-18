#include "CommandDomainStrings.h"

#include <string.h>

namespace CommandDomainStrings {

    const char *toString(CommandDomain domain) {
        switch (domain) {
            case CommandDomain::ROBOT: return "robot";
            case CommandDomain::SYSTEM: return "system";
            case CommandDomain::MOTION: return "motion";
            case CommandDomain::WIFI: return "wifi";
            case CommandDomain::UNKNOWN: return "unknown";
        }
        return "unknown";
    }

    bool fromString(const char *text, CommandDomain &domain) {
        if (text == nullptr)
            return false;
        if (strcmp(text, "robot") == 0) {
            domain = CommandDomain::ROBOT;
            return true;
        }
        if (strcmp(text, "system") == 0) {
            domain = CommandDomain::SYSTEM;
            return true;
        }
        if (strcmp(text, "motion") == 0) {
            domain = CommandDomain::MOTION;
            return true;
        }
        if (strcmp(text, "wifi") == 0) {
            domain = CommandDomain::WIFI;
            return true;
        }
        return false;
    }
}
