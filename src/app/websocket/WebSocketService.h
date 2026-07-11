#ifndef WEB_SOCKET_SERVICE_H
#define WEB_SOCKET_SERVICE_H
#include <stddef.h>
#include <WebSocketServer.h>
#include "../command/CommandService.h"

class WebSocketService {
private:
    WebSocketServer &_server;
    CommandService &_commands;
    static WebSocketService *_instance;
    static void _onMessage(const char *data, size_t len);
    void _handleMessage(const char *data, size_t len);
public:
    WebSocketService(WebSocketServer &server, CommandService &commands);
    void begin();
    void update();
};
#endif
