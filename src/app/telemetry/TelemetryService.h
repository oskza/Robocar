#ifndef TELEMETRY_SERVICE_H
#define TELEMETRY_SERVICE_H
#include <stddef.h>
#include <WebSocketServer.h>
#include "../../robot/Robot.h"
#include "../command/json/writer/RobotSnapshotJsonWriter.h"

class TelemetryService {
private:
    Robot &_robot;
    WebSocketServer &_server;
    uint32_t _intervalMs;
    uint32_t _lastBroadcastMs;
public:
    TelemetryService(Robot &robot, WebSocketServer &server, uint32_t intervalMs);
    void update(uint32_t nowMs);
};
#endif
