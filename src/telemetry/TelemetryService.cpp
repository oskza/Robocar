#include "TelemetryService.h"
#include <ArduinoJson.h>

static constexpr size_t TELEMETRY_BUFFER_SIZE = 2048;

TelemetryService::TelemetryService(Robot &robot, WebSocketServer &server, uint32_t intervalMs)
    : _robot(robot), _server(server), _intervalMs(intervalMs), _lastBroadcastMs(0) {}

void TelemetryService::update(uint32_t nowMs) {
    if (_intervalMs == 0 || nowMs - _lastBroadcastMs < _intervalMs)
        return;

    _lastBroadcastMs = nowMs;

    RobotSnapshot snapshot = _robot.getSnapshot();

    JsonDocument doc;
    doc["type"] = "telemetry";

    RobotSnapshotJsonWriter::write(doc["payload"].to<JsonObject>(), snapshot);

    char buffer[TELEMETRY_BUFFER_SIZE + 1];

    size_t required = measureJson(doc);

    if (required == 0 || required > TELEMETRY_BUFFER_SIZE)
        return;

    size_t written = serializeJson(doc, buffer, TELEMETRY_BUFFER_SIZE);

    if (written != required)
        return;

    buffer[written] = '\0';

    _server.broadcast(buffer);
}
