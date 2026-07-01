#ifndef WEB_SOCKET_TELEMETRY_H
#define WEB_SOCKET_TELEMETRY_H
#include <Arduino.h>
#include "RobotStateReader.h"
#include "RobotSnapshotJsonWriter.h"
#include <WebSocketServer.h>

class WebSocketTelemetry {
private:
    static constexpr size_t BUFFER_SIZE = 512;
    RobotStateReader &_stateReader;
    WebSocketServer &_webSocketServer;
    char _buffer[BUFFER_SIZE];
public:
    WebSocketTelemetry(RobotStateReader &stateReader, WebSocketServer &webSocketServer);
    void update(uint32_t now);
};
#endif
