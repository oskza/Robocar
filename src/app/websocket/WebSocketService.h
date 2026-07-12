#ifndef WEB_SOCKET_SERVICE_H
#define WEB_SOCKET_SERVICE_H
#include <stddef.h>
#include <WebSocketServer.h>
#include "../command/CommandProcessor.h"

class WebSocketService {
private:
    WebSocketServer &_server;
    CommandProcessor &_commands;
    static WebSocketService *_instance;
    static void _onMessage(const char *data, size_t len);
    void _handleMessage(const char *data, size_t len);
public:
    WebSocketService(WebSocketServer &server, CommandProcessor &commands);
    void begin();
    void update();
};
#endif
