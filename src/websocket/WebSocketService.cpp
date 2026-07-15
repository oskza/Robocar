#include "WebSocketService.h"

static constexpr size_t RESPONSE_BUFFER_SIZE = 2048;

WebSocketService *WebSocketService::_instance = nullptr;

WebSocketService::WebSocketService(WebSocketServer &server, CommandProcessor &commands)
    : _server(server), _commands(commands) { _instance = this; }

void WebSocketService::_onMessage(const char *data, size_t len) {
    if (_instance) _instance->_handleMessage(data, len);
}

void WebSocketService::_handleMessage(const char *data, size_t len) {
    char response[RESPONSE_BUFFER_SIZE];
    if (_commands.handle(data, len, response, sizeof(response)))
        _server.broadcast(response);
}

void WebSocketService::begin() { _server.begin(_onMessage); }

void WebSocketService::update() { _server.update(); }
