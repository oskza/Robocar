#include "WebSocketTelemetry.h"

WebSocketTelemetry::WebSocketTelemetry(RobotStateReader &stateReader, WebSocketServer &webSocketServer)
    : _stateReader(stateReader), _webSocketServer(webSocketServer) {}

void WebSocketTelemetry::update(uint32_t now) {
    if (!_webSocketServer.hasClients())
        return;
    RobotSnapshot snapshot = _stateReader.read(now);
    if (!RobotSnapshotJsonWriter::write(snapshot, _buffer, sizeof(_buffer)))
        return;
    _webSocketServer.broadcast(_buffer);
}
