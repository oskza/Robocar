#include "WebSocketCommands.h"
#include "controllers/WebSocketController.h"

extern WebSocketController wsController;

static void handleGetConfig(JsonDocument &doc) {
    JsonObject json = doc["cfg"].to<JsonObject>();
    WebSocketConfig cfg;
    wsController.getConfig(cfg);
    json["maxClients"] = cfg.maxClients;
    json["interval"] = cfg.intervalMs;
}

static void handleUpdateConfig(JsonDocument &doc) {
    JsonObject payload = doc["payload"];
    WebSocketConfig cfg;
    cfg.maxClients = payload["maxClients"];
    cfg.intervalMs = payload["interval"];
    wsController.updateConfig(cfg);
}

CommandEntry systemCommands[] = {
    { "get-config", handleGetConfig },
    { "update-config", handleUpdateConfig },
    { "reset", [](JsonDocument &) { wsController.reset(); } }
};

const size_t systemCommandCount = sizeof(systemCommands) / sizeof(CommandEntry);
