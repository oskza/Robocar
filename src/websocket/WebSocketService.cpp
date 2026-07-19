#include "WebSocketService.h"

WebSocketService *WebSocketService::_instance = nullptr;

WebSocketService::WebSocketService(WebSocketServer &server, CommandProcessor &commands)
    : _server(server),
      _commands(commands),
      _started(false) {}

void WebSocketService::_onMessage(AsyncWebSocketClient *client, const uint8_t *data, size_t length) {
    if (_instance != nullptr)
        _instance->_handleMessage(client, data, length);
}

void WebSocketService::_handleMessage(AsyncWebSocketClient *client, const uint8_t *data, size_t length) {
    if (client == nullptr)
        return;
    char responseBuffer[RESPONSE_BUFFER_SIZE];
    if (!_commands.handle(data, length, responseBuffer, sizeof(responseBuffer)))
        return;
    _server.send(client, responseBuffer);
}

bool WebSocketService::begin() {
    if (_started)
        return true;
    if (_instance != nullptr && _instance != this)
        return false;
    _instance = this;
    if (!_server.begin(_onMessage)) {
        _instance = nullptr;
        return false;
    }
    _started = true;
    return true;
}

void WebSocketService::update() {
    if (_started)
        _server.update();
}

bool WebSocketService::isStarted() const {
    return _started;
}
