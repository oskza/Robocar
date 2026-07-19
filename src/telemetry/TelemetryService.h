#ifndef TELEMETRY_SERVICE_H
#define TELEMETRY_SERVICE_H
#include <stddef.h>
#include <WebSocketServer.h>
#include "robot/Robot.h"

class TelemetryService {
private:
    static constexpr size_t BUFFER_SIZE = 2048;
    Robot &_robot;
    WebSocketServer &_server;
    uint32_t _intervalMs;
    uint32_t _lastBroadcastMs;
    char _buffer[BUFFER_SIZE];
public:
    TelemetryService(Robot &robot, WebSocketServer &server, uint32_t intervalMs);
    void update(uint32_t nowMs);
    uint32_t getIntervalMs() const;
    void setIntervalMs(uint32_t intervalMs);
};
#endif
