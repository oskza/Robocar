#ifndef COMMAND_DOMAIN_STRINGS_H
#define COMMAND_DOMAIN_STRINGS_H
#include "CommandDomain.h"

namespace CommandDomainStrings {
    const char *toString(CommandDomain domain);
    bool fromString(const char *text, CommandDomain &domain);
}
#endif
