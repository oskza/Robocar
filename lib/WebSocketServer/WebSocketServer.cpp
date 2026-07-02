#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(uint16_t port, const char *path, uint8_t maxClients)
    : _server(port),
      _ws(path),
      _maxClients(maxClients),
      _messageCallback(nullptr) {}

void WebSocketServer::begin() {
    _ws.onEvent([this](
            AsyncWebSocket*, AsyncWebSocketClient *client,
            AwsEventType type, void *arg, uint8_t *data, size_t len) {
        _handleEvent(client, type, arg, data, len);
    });
    _server.addHandler(&_ws);
    _server.begin();
}

void WebSocketServer::update() { _ws.cleanupClients(_maxClients); }

bool WebSocketServer::hasClients() const { return getClientCount() > 0; }

uint8_t WebSocketServer::getClientCount() const { return (uint8_t)_ws.count(); }

bool WebSocketServer::send(AsyncWebSocketClient *client, const char *message, size_t length) {
    if (!client || !message || length == 0)
        return false;
    client->text(message, length);
    return true;
}

bool WebSocketServer::broadcast(const char *message) {
    if (!message)
        return false;
    return broadcast(message, strlen(message));
}

bool WebSocketServer::broadcast(const char *message, size_t length) {
    if (!message || length == 0 || !hasClients())
        return false;
    _ws.textAll(message, length);
    return true;
}

void WebSocketServer::setMessageCallback(MessageCallback callback) { _messageCallback = callback; }

void WebSocketServer::_handleEvent(AsyncWebSocketClient *client, AwsEventType type,
                                    void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            _handleConnect(client);
            break;
        case WS_EVT_DATA:
            _handleData(arg, data, len);
            break;
        default:
            break;
    }
}

void WebSocketServer::_handleConnect(AsyncWebSocketClient *client) {
    if (!client)
        return;
    client->setCloseClientOnQueueFull(false);
    client->ping();
}

void WebSocketServer::_handleData(void *arg, uint8_t *data, size_t len) {
    if (!_messageCallback)
        return;
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (!info || !info->final
            || info->index != 0
            || info->len != len
            || info->opcode != WS_TEXT)
        return;
    _messageCallback((const char *)data, len);
}
