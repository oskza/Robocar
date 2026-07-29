#include "TelemetryService.h"
#include <ArduinoJson.h>
#include "robot/json/writer/RobotSnapshotJsonWriter.h"

TelemetryService::TelemetryService(Robot &robot, WebSocketServer &server, uint32_t intervalMs)
    : _robot(robot),
      _server(server),
      _intervalMs(intervalMs),
      _broadcastTimer{},
      _buffer{} {}

void TelemetryService::update(uint32_t nowMs) {
    if (_intervalMs == 0 || !_server.hasClients()) {
        _broadcastTimer.stop();
        return;
    }
    if (!_broadcastTimer.poll(nowMs, _intervalMs))
        return;
    const RobotSnapshot snapshot = _robot.getSnapshot();
    JsonDocument doc;
    doc["type"] = "telemetry";
    RobotSnapshotJsonWriter::write(doc["payload"].to<JsonObject>(), snapshot);
    const size_t requiredCapacity = measureJson(doc) + 1;
    if (requiredCapacity > sizeof(_buffer))
        return;
    const size_t written = serializeJson(doc, _buffer, sizeof(_buffer));
    if (written == 0)
        return;
    _server.broadcast(_buffer, written);
}

uint32_t TelemetryService::getIntervalMs() const { return _intervalMs; }

void TelemetryService::setIntervalMs(uint32_t intervalMs) {
    if (_intervalMs == intervalMs)
        return;
    _intervalMs = intervalMs;
    _broadcastTimer.stop();
}
