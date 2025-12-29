#include "WebSocketController.h"

WebSocketController* WebSocketController::_instance = nullptr;

WebSocketController::WebSocketController(AsyncWebSocket &ws, Timer &timer, uint8_t maxClients) 
                                        : WebSocketController(ws, timer, nullptr, maxClients) {}

WebSocketController::WebSocketController(AsyncWebSocket &ws, Timer &timer, CommandCallback onCmd, uint8_t maxClients)
                                        : _ws(ws), _timer(timer), _onCmd(onCmd), _maxClients(maxClients) { _instance = this; }


void WebSocketController::_handleConnect(AsyncWebSocketClient *client) {
    client->setCloseClientOnQueueFull(false);
    client->ping();
}

void WebSocketController::_handleData(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (!(info->final && info->opcode == WS_TEXT))
        return;
    data[len] = 0;
    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, data)) {
        client->text("{\"type\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }
    const char* type = doc["type"] | "";
    if (!type) 
        return;
    if (strcmp(type, "command") == 0) {
        if (!_onCmd)
            return;
        _onCmd(doc);
    }
}

void WebSocketController::_middleware(AsyncWebServerRequest *request, ArMiddlewareNext next) {
    if (_ws.count() >= _maxClients) {
        request->send(503, "text/plain", "Server busy");
        return;
    }
    next();
}

void WebSocketController::_onEvent(AsyncWebSocket*, AsyncWebSocketClient *client, 
                                    AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            _handleConnect(client);
            break;
        case WS_EVT_DISCONNECT:
            break;
        case WS_EVT_ERROR:
            break;
        case WS_EVT_PONG:
            break;
        case WS_EVT_DATA:
            _handleData(client, arg, data, len);
            break;
    }
}

void WebSocketController::_middlewareStatic(AsyncWebServerRequest *request, ArMiddlewareNext next) {
    if (_instance)
        _instance->_middleware(request, next);
}

void WebSocketController::_onEventStatic(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                            AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (_instance)
        _instance->_onEvent(server, client, type, arg, data, len);
}

void WebSocketController::init(AsyncWebServer &server, uint32_t msInterval) {
    _ws.onEvent(_onEventStatic);
    server.addHandler(&_ws).addMiddleware(_middlewareStatic);
    _timer.setTimeout(msInterval);
    _timer.start();
}

void WebSocketController::init(AsyncWebServer &server, CommandCallback cb, uint32_t msInterval) {
    WebSocketController::init(server, msInterval);
    setCommandCallback(cb);
}

void WebSocketController::tick() {
    if (!_timer.tick())
        return;
    _ws.cleanupClients(_maxClients);
    _timer.refresh();
}

void WebSocketController::sendAll(JsonDocument &doc) {
    char buffer[512];
    serializeJson(doc, buffer, sizeof(buffer));
    _ws.textAll(buffer);
}

size_t WebSocketController::getClientsCount() const { return _ws.count(); }

bool WebSocketController::hasClients() const { return getClientsCount() > 0; }

void WebSocketController::setCommandCallback(CommandCallback cb) { _onCmd = cb; }
