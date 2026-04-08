#include "SystemCommands.h"
#include "controllers/SystemController.h"

extern SystemController systemController;

static void handleGetConfig(JsonDocument &doc) {
    JsonObject json = doc["cfg"].to<JsonObject>();
    SystemConfig cfg;
    systemController.getConfig(cfg);
    json["statusInterval"] = cfg.statusReportIntervalMs;
}

static void handleUpdateConfig(JsonDocument &doc) {
    JsonObject payload = doc["payload"];
    SystemConfig cfg;
    cfg.statusReportIntervalMs = payload["statusInterval"];
    systemController.updateConfig(cfg);
}

CommandEntry systemCommands[] = {
    { "get-config", handleGetConfig },
    { "update-config", handleUpdateConfig },
    { "reset-config", [](JsonDocument &) { systemController.resetConfig(); } },
    { "factory-reset", [](JsonDocument &) { systemController.factoryReset(); } },
    { "restart", [](JsonDocument &) { systemController.restart(); } }
};

const size_t systemCommandCount = sizeof(systemCommands) / sizeof(CommandEntry);
