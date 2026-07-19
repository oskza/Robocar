#ifndef WEB_SOCKET_SERVICE_H
#define WEB_SOCKET_SERVICE_H
#include <WebSocketServer.h>
#include <stddef.h>
#include "command/CommandProcessor.h"

class WebSocketService {
private:
    static constexpr size_t RESPONSE_BUFFER_SIZE = 2048;
    WebSocketServer &_server;
    CommandProcessor &_commands;
    bool _started;
    static WebSocketService *_instance;
    static void _onMessage(AsyncWebSocketClient *client, const uint8_t *data, size_t length);
    void _handleMessage(AsyncWebSocketClient *client, const uint8_t *data, size_t length);
public:
    WebSocketService(WebSocketServer &server, CommandProcessor &commands);
    bool begin();
    void update();
    bool isStarted() const;
};
#endif
