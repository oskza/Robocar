#include "TelemetryService.h"
#include <ArduinoJson.h>
#include "robot/json/writer/RobotSnapshotJsonWriter.h"

TelemetryService::TelemetryService(Robot &robot, WebSocketServer &server, uint32_t intervalMs)
    : _robot(robot),
      _server(server),
      _intervalMs(intervalMs),
      _lastBroadcastMs(0),
      _buffer{} {}

void TelemetryService::update(uint32_t nowMs) {
    if (_intervalMs == 0
            || !_server.hasClients()
            || nowMs - _lastBroadcastMs < _intervalMs)
        return;
    _lastBroadcastMs = nowMs;
    const RobotSnapshot snapshot = _robot.getSnapshot();
    JsonDocument document;
    document["type"] = "telemetry";
    RobotSnapshotJsonWriter::write(document["payload"].to<JsonObject>(), snapshot);
    const size_t requiredCapacity = measureJson(document) + 1;
    if (requiredCapacity > sizeof(_buffer))
        return;
    const size_t written = serializeJson(document, _buffer, sizeof(_buffer));
    if (written == 0)
        return;
    _server.broadcast(_buffer, written);
}

uint32_t TelemetryService::getIntervalMs() const { return _intervalMs; }

void TelemetryService::setIntervalMs(uint32_t intervalMs) { _intervalMs = intervalMs; }
