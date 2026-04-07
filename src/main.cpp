#include <Arduino.h>
#include "nvs_flash.h"
#include "storage/DeviceStorage.h"
#include "controllers/NetworkController.h"
#include "controllers/WebSocketController.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED                   115200
#endif

#define SERVER_PORT                     80
#define WEBSOCKET_PATH                  "/ws"

DeviceStorage deviceStorage;
Timer statusReportTimer;

NetworkStorage networkStorage;
Timer networkTimer;
NetworkController networkController(networkStorage, networkTimer);

AsyncWebSocket ws(WEBSOCKET_PATH);
WebSocketStorage wsStorage;
Timer wsTimer;
WebSocketController wsController(ws, wsStorage, wsTimer);

AsyncWebServer server(SERVER_PORT);

void factoryReset() {
    nvs_flash_erase();
    nvs_flash_init();
    ESP.restart();
}

void handleCommand(JsonDocument &doc) {
    const char *cmd = doc["cmd"] | "";
    if (!cmd) 
        return;
    if (strcmp(cmd, "reset") == 0) {
        factoryReset();
        return;
    }
    if (strcmp(cmd, "restart") == 0) {
        ESP.restart();
        return;
    }
}

void getHeapStatus(JsonObject &target) {
    target["free"] = ESP.getFreeHeap();
    target["total"] = ESP.getHeapSize();
    target["maxAlloc"] = ESP.getMaxAllocHeap();
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
    payload["uptime"] = millis();
    
    JsonObject heap = payload["heap"].to<JsonObject>();
    getHeapStatus(heap);

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

    deviceStorage.begin();
    statusReportTimer.setTimeout(deviceStorage.loadStatusReportIntervalMs());
    statusReportTimer.start();

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

    if (statusReportTimer.tick()) {
        statusReportTimer.refresh();
        if (wsController.hasClients()) {
            StaticJsonDocument<512> status;
            createStatus(status);
            wsController.sendAll(status);
        }
    }
}
