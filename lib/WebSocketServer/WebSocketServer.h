#ifndef WEB_SOCKET_SERVER_H
#define WEB_SOCKET_SERVER_H
#include <ESPAsyncWebServer.h>

class WebSocketServer {
public:
    using MessageHandler = void (*)(const char *data, size_t len);
    explicit WebSocketServer(uint16_t port = 80, const char *path = "/ws",
                                uint8_t maxClients = 4);
    void begin(MessageHandler messageHandler = nullptr);
    void onMessage(MessageHandler handler);
    void update();
    bool hasClients() const;
    uint8_t getClientCount() const;
    bool send(AsyncWebSocketClient *client, const char *message, size_t length);
    bool broadcast(const char *message);
    bool broadcast(const char *message, size_t length);
private:
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    uint8_t _maxClients;
    MessageHandler _messageHandler;
    void _handleEvent(AsyncWebSocketClient *client, AwsEventType type,
                        void *arg, uint8_t *data, size_t len);
    void _handleConnect(AsyncWebSocketClient *client);
    void _handleData(void *arg, uint8_t *data, size_t len);
};
#endif
