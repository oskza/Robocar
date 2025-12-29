#ifndef WEBSOCKET_CONTROLLER_H
#define WEBSOCKET_CONTROLLER_H
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Timer.h>

typedef void (*CommandCallback)(JsonDocument &doc);

class WebSocketController {
private:
    AsyncWebSocket &_ws;
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
    WebSocketController(AsyncWebSocket &ws, Timer &timer, uint8_t maxClients);
    WebSocketController(AsyncWebSocket &ws, Timer &timer, CommandCallback onCmd, uint8_t maxClients);
    void init(AsyncWebServer &server, uint32_t msInterval);
    void init(AsyncWebServer &server, CommandCallback cb, uint32_t msInterval);
    void tick();
    void sendAll(JsonDocument &doc);
    size_t getClientsCount() const;
    bool hasClients() const;
    void setCommandCallback(CommandCallback cb);
};
#endif
