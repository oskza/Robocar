#include "CommandDispatcher.h"
#include "commands/SystemCommands.h"
#include "commands/NetworkCommands.h"
#include "commands/DriveCommands.h"

static DomainEntry domains[] = {
    { "system", systemCommands, systemCommandCount },
    { "network", networkCommands, networkCommandCount },
    { "drive", driveCommands, driveCommandCount }
};

static bool executeCommand(DomainEntry *domain, const char *cmd, JsonDocument &doc) {
    for (size_t i = 0; i < domain->size; i++) {
        if (strcmp(cmd, domain->commands[i].cmd) == 0) {
            domain->commands[i].handler(doc);
            return true;
        }
    }
    return false;
}

void handleCommand(JsonDocument &doc) {
    const char *domain = doc["domain"] | "";
    const char *cmd = doc["cmd"] | "";
    if (!domain || !cmd) 
        return;
    for (auto &d : domains) {
        if (strcmp(domain, d.domain) == 0) {
            if (!executeCommand(&d, cmd, doc)) {/** TODO: unknown command */}
            return;
        }
    }
}
