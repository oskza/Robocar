#include <Arduino.h>
#include "SystemController.h"
#include "CommandDispatcher.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED                   115200
#endif

#define SERVER_PORT                     80
#define WEBSOCKET_PATH                  "/ws"

SystemStorage systemStorage;
Timer statusReportTimer;
SystemController systemController(systemStorage, statusReportTimer);

NetworkStorage networkStorage;
Timer networkTimer;
NetworkController networkController(networkStorage, networkTimer);

AsyncWebSocket ws(WEBSOCKET_PATH);
WebSocketStorage wsStorage;
Timer wsTimer;
WebSocketController wsController(ws, wsStorage, wsTimer);

AsyncWebServer server(SERVER_PORT);

void parseSystemStatus(const SystemStatus &status, JsonObject &target) {
    target["uptime"] = status.uptime;
    JsonObject heap = target["heap"].to<JsonObject>();
    heap["free"] = status.heap.free;
    heap["total"] = status.heap.total;
    heap["maxAlloc"] = status.heap.maxAlloc;
}

void parseNetworkStatus(const NetworkStatus &status, JsonObject &target) {
    target["mode"] = status.mode;
    if (strcmp(status.mode, "ap") == 0) {
        JsonObject ap = target["ap"].to<JsonObject>();
        ap["clients"] = status.ap.clients;
        return;
    }
    JsonObject sta = target["sta"].to<JsonObject>();
    sta["connected"] = status.sta.connected;
    if (status.sta.connected) {
        sta["rssi"] = status.sta.rssi;
    }
}

void parseWSStatus(const WSStatus &status, JsonObject &target) {
    target["clients"] = status.clients;
}

void createStatus(JsonDocument &doc) {
    doc["type"] = "status";
    JsonObject payload = doc["payload"].to<JsonObject>();
    
    JsonObject system = payload["system"].to<JsonObject>();
    SystemStatus systemStatus;
    systemController.getStatus(systemStatus);
    parseSystemStatus(systemStatus, system);

    JsonObject network = payload["network"].to<JsonObject>();
    NetworkStatus netStatus;
    networkController.getStatus(netStatus);
    parseNetworkStatus(netStatus, network);

    JsonObject ws = payload["ws"].to<JsonObject>();
    WSStatus wsStatus;
    wsController.getStatus(wsStatus);
    parseWSStatus(wsStatus, ws);
}

void setup() {
    // Serial.begin(MONITOR_SPEED);

    systemController.init();
    if (networkController.init())
        networkController.connect();
    else
        networkController.enableAP();
    wsController.init(server, handleCommand);
    server.begin();
}

void loop() {
    if (networkController.tick()) {/** TODO: ... */}
    wsController.tick();
    if (systemController.tick() && wsController.hasClients()) {
        StaticJsonDocument<512> status;
        createStatus(status);
        wsController.sendAll(status);
    }
}
