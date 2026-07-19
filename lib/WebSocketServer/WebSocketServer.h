#ifndef WEB_SOCKET_SERVER_H
#define WEB_SOCKET_SERVER_H
#include <ESPAsyncWebServer.h>
#include <stddef.h>

class WebSocketServer {
public:
    using MessageHandler = void (*)(AsyncWebSocketClient *client, const uint8_t *data, size_t length);
    explicit WebSocketServer(uint16_t port = 80, const char *path = "/ws", uint8_t maxClients = 4);
    bool begin(MessageHandler messageHandler = nullptr);
    void onMessage(MessageHandler handler);
    void update();
    bool isStarted() const;
    bool hasClients() const;
    uint8_t getClientCount() const;
    bool send(AsyncWebSocketClient *client, const char *message, size_t length);
    bool send(AsyncWebSocketClient *client, const char *message);
    bool broadcast(const char *message, size_t length);
    bool broadcast(const char *message);
private:
    AsyncWebServer _server;
    AsyncWebSocket _webSocket;
    uint8_t _maxClients;
    MessageHandler _messageHandler;
    bool _started;
    void _handleEvent(AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t length);
    void _handleConnect(AsyncWebSocketClient *client);
    void _handleData(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t length);
};
#endif
