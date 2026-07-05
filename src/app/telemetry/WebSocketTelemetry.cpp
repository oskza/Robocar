#include "WebSocketTelemetry.h"
#include "../serialization/json/RobotSnapshotJsonSerializer.h"

WebSocketTelemetry::WebSocketTelemetry(WebSocketServer &webSocketServer, SnapshotReader snapshotReader)
    : _webSocketServer(webSocketServer), _snapshotReader(snapshotReader) {}

void WebSocketTelemetry::update(uint32_t nowMs) {
    if (!_webSocketServer.hasClients() || !_snapshotReader)
        return;
    RobotSnapshot snapshot = _snapshotReader(nowMs);
    if (!RobotSnapshotJsonSerializer::write(snapshot, _buffer, sizeof(_buffer)))
        return;
    _webSocketServer.broadcast(_buffer);
}
