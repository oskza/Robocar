#include "SystemCommandJsonDeserializer.h"
#include <ArduinoJson.h>
#include <string.h>

SystemCommandReadResult SystemCommandJsonDeserializer::read(const char *data, size_t len) {
    if (!data || len == 0)
        return SystemCommandReadResult::failure();
    JsonDocument doc;
    if (deserializeJson(doc, data, len))
        return SystemCommandReadResult::failure();
    JsonVariantConst cmdValue = doc["cmd"];
    if (!cmdValue.is<const char *>())
        return SystemCommandReadResult::failure();
    const char *cmd = cmdValue.as<const char *>();
    if (strcmp(cmd, "ping") == 0)
        return SystemCommandReadResult::success(SystemCommand::ping());
    if (strcmp(cmd, "restart") == 0)
        return SystemCommandReadResult::success(SystemCommand::restart());
    if (strcmp(cmd, "factoryReset") == 0)
        return SystemCommandReadResult::success(SystemCommand::factoryReset());
    return SystemCommandReadResult::failure();
}
