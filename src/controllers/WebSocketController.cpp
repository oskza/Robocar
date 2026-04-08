#include "WebSocketController.h"

WebSocketController* WebSocketController::_instance = nullptr;

WebSocketController::WebSocketController(AsyncWebSocket &ws, WebSocketStorage &storage, Timer &timer)
                                        : _ws(ws), _storage(storage), _timer(timer), 
                                            _onCmd(nullptr), _maxClients(1) { _instance = this; }


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
    if (strcmp(type, "command") == 0 && _onCmd)
        _onCmd(doc);
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
        case WS_EVT_DATA:
            _handleData(client, arg, data, len);
            break;        
        case WS_EVT_DISCONNECT:
            break;
        case WS_EVT_ERROR:
            break;
        case WS_EVT_PONG:
            break;
    }
}

void WebSocketController::_middlewareStatic(AsyncWebServerRequest *request, ArMiddlewareNext next) {
    if (_instance) _instance->_middleware(request, next);
}

void WebSocketController::_onEventStatic(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                            AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (_instance) _instance->_onEvent(server, client, type, arg, data, len);
}

void WebSocketController::init(AsyncWebServer &server, CommandCallback cb) {
    _storage.begin();
    _ws.onEvent(_onEventStatic);
    server.addHandler(&_ws).addMiddleware(_middlewareStatic);
    onCommand(cb);
    _maxClients = _storage.loadMaxClients();
    _timer.setTimeout(_storage.loadIntervalMs());
    _timer.start();
}

void WebSocketController::tick() {
    if (!_timer.tick())
        return;
    _timer.refresh();
    _ws.cleanupClients(_maxClients);
}

void WebSocketController::sendAll(JsonDocument &doc) {
    char buffer[512];
    serializeJson(doc, buffer, 512);
    _ws.textAll(buffer);
}

void WebSocketController::reset() {
    _storage.reset();
    _maxClients = WebSocketDefaults::maxClients;
    _timer.setTimeout(WebSocketDefaults::intervalMs);
}

void WebSocketController::getStatus(WSStatus &target) const { target.clients = getClientsCount(); }

void WebSocketController::getConfig(WebSocketConfig &target) const { _storage.loadConfig(target); }

size_t WebSocketController::getClientsCount() const { return _ws.count(); }

bool WebSocketController::hasClients() const { return getClientsCount() > 0; }

void WebSocketController::onCommand(CommandCallback cb) { _onCmd = cb; }

void WebSocketController::updateConfig(WebSocketConfig &cfg) {
    updateMaxClients(cfg.maxClients);
    updateIntervalMs(cfg.intervalMs);
}

void WebSocketController::updateMaxClients(uint8_t clients) {
    if (_maxClients == clients)
        return;
    _maxClients = clients;
    _storage.saveMaxClients(clients);
}

void WebSocketController::updateIntervalMs(uint32_t ms) {
    if (_timer.getTimeout() == ms)
        return;
    _timer.setTimeout(ms);
    _storage.saveIntervalMs(ms);
}
