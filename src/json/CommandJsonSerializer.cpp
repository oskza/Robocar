#include "CommandJsonSerializer.h"
#include <ArduinoJson.h>
#include "robot/json/writer/RobotSnapshotJsonWriter.h"
#include "system/json/writer/SystemSnapshotJsonWriter.h"
#include "robot/json/writer/RobotConfigJsonWriter.h"
#include "motion/json/writer/MotionSnapshotJsonWriter.h"
#include "motion/json/writer/MotionConfigJsonWriter.h"
#include "network/json/writer/WifiSnapshotJsonWriter.h"
#include "network/json/writer/WifiConfigJsonWriter.h"
#include "network/json/writer/WifiCredentialsJsonWriter.h"

const char *CommandJsonSerializer::_statusToString(CommandResponseStatus status) {
    switch (status) {
        case CommandResponseStatus::OK: return "ok";
        case CommandResponseStatus::ERROR: return "error";
    }
    return "unknown";
}

const char *CommandJsonSerializer::_typeToString(CommandResponseType type) {
    switch (type) {
        case CommandResponseType::ACK: return "ack";
        case CommandResponseType::ERROR: return "error";
        case CommandResponseType::ROBOT_STATUS: return "robotStatus";
        case CommandResponseType::MOTION_STATUS: return "motionStatus";
        case CommandResponseType::ROBOT_CONFIG: return "robotConfig";
        case CommandResponseType::MOTION_CONFIG: return "motionConfig";
        case CommandResponseType::WIFI_CONFIG: return "wifiConfig";
        case CommandResponseType::WIFI_STATION_CREDENTIALS: return "wifiStationCredentials";
        case CommandResponseType::WIFI_ACCESS_POINT_CREDENTIALS: return "wifiAccessPointCredentials";
    }
    return "unknown";
}

const char *CommandJsonSerializer::_errorToString(CommandError error) {
    switch (error) {
        case CommandError::NONE: return "none";
        case CommandError::INVALID_COMMAND: return "invalidCommand";
        case CommandError::INVALID_PAYLOAD: return "invalidPayload";
        case CommandError::STORAGE_ERROR: return "storageError";
        case CommandError::SYSTEM_ERROR: return "systemError";
        case CommandError::WIFI_INVALID_CREDENTIALS: return "wifiInvalidCredentials";
        case CommandError::WIFI_NOT_CONNECTED: return "wifiNotConnected";
    }
    return "unknown";
}

bool CommandJsonSerializer::serialize(
    const CommandResponse &response,
    char *buffer,
    size_t size
) {
    if (!buffer || size == 0)
        return false;

    JsonDocument doc;

    doc["type"] = "res";
    doc["id"] = response.id;
    doc["status"] = _statusToString(response.status);
    doc["response"] = _typeToString(response.type);
    doc["error"] = _errorToString(response.error);

    JsonObject payload = doc["payload"].to<JsonObject>();

    switch (response.type) {
        case CommandResponseType::ROBOT_STATUS:
            RobotSnapshotJsonWriter::write(payload, response.payload.robot);
            break;

        case CommandResponseType::MOTION_STATUS:
            MotionSnapshotJsonWriter::write(payload, response.payload.motion);
            break;

        case CommandResponseType::ROBOT_CONFIG:
            RobotConfigJsonWriter::write(payload, response.payload.robotConfig);
            break;

        case CommandResponseType::MOTION_CONFIG:
            MotionConfigJsonWriter::write(payload, response.payload.motionConfig);
            break;

        case CommandResponseType::WIFI_CONFIG:
            WifiConfigJsonWriter::write(payload, response.payload.wifiConfig);
            break;

        case CommandResponseType::WIFI_STATION_CREDENTIALS:
        case CommandResponseType::WIFI_ACCESS_POINT_CREDENTIALS:
            WifiCredentialsJsonWriter::write(payload, response.payload.wifiCredentials);
            break;

        case CommandResponseType::ACK:
        case CommandResponseType::ERROR:
            break;
    }

    const size_t written = serializeJson(doc, buffer, size);
    return written > 0 && written < size;
}
