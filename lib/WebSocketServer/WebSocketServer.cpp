#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(uint16_t port, const char *path) : _ws(path), _server(port) {}

void WebSocketServer::begin() {
    _server.addHandler(&_ws);
    _server.begin();
}

void WebSocketServer::update() { _ws.cleanupClients(); }

bool WebSocketServer::hasClients() const { return _ws.count() > 0; }

bool WebSocketServer::broadcast(const char *message) {
    if (!message || !hasClients())
        return false;
    _ws.textAll(message);
    return true;
}
