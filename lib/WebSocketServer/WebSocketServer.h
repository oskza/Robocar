#ifndef WEB_SOCKET_SERVER_H
#define WEB_SOCKET_SERVER_H
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class WebSocketServer {
private:
    AsyncWebSocket _ws;
    AsyncWebServer _server;
public:
    explicit WebSocketServer(uint16_t port = 80, const char *path = "/ws");
    void begin();
    void update();
    bool hasClients() const;
    bool broadcast(const char *message);
};
#endif
