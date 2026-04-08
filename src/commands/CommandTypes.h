#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H
#include <ArduinoJson.h>

typedef void (*CommandHandler)(JsonDocument &doc);

struct CommandEntry {
    const char *cmd;
    CommandHandler handler;
};

struct DomainEntry {
    const char *domain;
    CommandEntry *commands;
    size_t size;
};
#endif
