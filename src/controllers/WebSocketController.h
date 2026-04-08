#ifndef WEBSOCKET_CONTROLLER_H
#define WEBSOCKET_CONTROLLER_H
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Timer.h>
#include "storage/WebSocketStorage.h"

struct WSStatus {
    uint8_t clients;
};

typedef void (*CommandCallback)(JsonDocument &doc);

/** TODO: remove json dependency */
class WebSocketController {
private:
    AsyncWebSocket &_ws;
    WebSocketStorage &_storage;
    Timer &_timer;
    CommandCallback _onCmd;
    uint8_t _maxClients;
    void _handleConnect(AsyncWebSocketClient *client);
    void _handleData(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len);
    void _middleware(AsyncWebServerRequest *request, ArMiddlewareNext next);
    void _onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                    AwsEventType type, void *arg, uint8_t *data, size_t len);
    static void _middlewareStatic(AsyncWebServerRequest *request, ArMiddlewareNext next);
    static void _onEventStatic(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                AwsEventType type, void *arg, uint8_t *data, size_t len);
    static WebSocketController *_instance;
public:
    WebSocketController(AsyncWebSocket &ws, WebSocketStorage &storage, Timer &timer);
    void init(AsyncWebServer &server, CommandCallback cb);
    void tick();
    void sendAll(JsonDocument &doc);
    size_t getClientsCount() const;
    bool hasClients() const;
    void onCommand(CommandCallback cb);
    void reset();
    void getStatus(WSStatus &target) const;
    void getConfig(WebSocketConfig &target) const;
    void updateConfig(WebSocketConfig &cfg);
    void updateMaxClients(uint8_t clients);
    void updateIntervalMs(uint32_t ms);
};
#endif
