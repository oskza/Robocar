#ifndef WEB_SOCKET_TELEMETRY_H
#define WEB_SOCKET_TELEMETRY_H
#include <WebSocketServer.h>
#include "../domain/RobotSnapshot.h"

class WebSocketTelemetry {
public:
    using SnapshotReader = RobotSnapshot (*)(uint32_t uptimeMs);
    WebSocketTelemetry(WebSocketServer &webSocketServer, SnapshotReader snapshotReader);
    void update(uint32_t nowMs);
private:
    static constexpr size_t BUFFER_SIZE = 512;
    WebSocketServer &_webSocketServer;
    SnapshotReader _snapshotReader;
    char _buffer[BUFFER_SIZE];
};
#endif
