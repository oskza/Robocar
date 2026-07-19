#include "WebSocketServer.h"
#include <string.h>

WebSocketServer::WebSocketServer(uint16_t port, const char *path, uint8_t maxClients)
    : _server(port),
      _webSocket(path),
      _maxClients(maxClients),
      _messageHandler(nullptr),
      _started(false) {}

void WebSocketServer::_handleEvent(AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t length) {
    switch (type) {
        case WS_EVT_CONNECT:
            _handleConnect(client);
            break;
        case WS_EVT_DATA:
            _handleData(client, arg, data, length);
            break;
        case WS_EVT_DISCONNECT:
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void WebSocketServer::_handleConnect(AsyncWebSocketClient *client) {
    if (client == nullptr)
        return;
    if (getClientCount() > _maxClients) {
        client->close();
        return;
    }
    client->setCloseClientOnQueueFull(false);
    client->ping();
}

void WebSocketServer::_handleData(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t length) {
    if (client == nullptr
            || _messageHandler == nullptr
            || data == nullptr
            || length == 0)
        return;
    const AwsFrameInfo *info = static_cast<AwsFrameInfo*>(arg);
    if (info == nullptr
            || !info->final
            || info->index != 0
            || info->len != length
            || info->opcode != WS_TEXT)
        return;
    _messageHandler(client, data, length);
}

bool WebSocketServer::begin(MessageHandler messageHandler) {
    if (_started) {
        if (messageHandler != nullptr)
            _messageHandler = messageHandler;
        return true;
    }
    _messageHandler = messageHandler;
    _webSocket.onEvent(
        [this](
            AsyncWebSocket *,
            AsyncWebSocketClient *client,
            AwsEventType type,
            void *arg,
            uint8_t *data,
            size_t length
        ) { _handleEvent(client, type, arg, data, length); }
    );
    _server.addHandler(&_webSocket);
    _server.begin();
    _started = true;
    return true;
}

void WebSocketServer::onMessage(MessageHandler handler) { _messageHandler = handler; }

void WebSocketServer::update() {
    if (!_started)
        return;
    _webSocket.cleanupClients(_maxClients);
}

bool WebSocketServer::isStarted() const { return _started; }

bool WebSocketServer::hasClients() const { return getClientCount() > 0; }

uint8_t WebSocketServer::getClientCount() const { return (uint8_t)_webSocket.count(); }

bool WebSocketServer::send(AsyncWebSocketClient *client, const char *message, size_t length) {
    if (!_started
            || client == nullptr
            || message == nullptr
            || length == 0
            || client->status() != WS_CONNECTED)
        return false;
    client->text(message, length);
    return true;
}

bool WebSocketServer::send(AsyncWebSocketClient *client, const char *message) {
    return (message != nullptr) ? send(client, message, strlen(message)) : false;
}

bool WebSocketServer::broadcast(const char *message, size_t length) {
    if (!_started
            || message == nullptr
            || length == 0
            || !hasClients())
        return false;
    _webSocket.textAll(message, length);
    return true;
}

bool WebSocketServer::broadcast(const char *message) {
    return (message != nullptr) ? broadcast(message, strlen(message)) : false;
}
