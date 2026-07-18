#include "CommandJsonSerializer.h"
#include <ArduinoJson.h>
#include "motion/json/writer/MotionConfigJsonWriter.h"
#include "motion/json/writer/MotionSnapshotJsonWriter.h"
#include "network/json/writer/WifiConfigJsonWriter.h"
#include "network/json/writer/WifiCredentialsJsonWriter.h"
#include "robot/json/writer/RobotConfigJsonWriter.h"
#include "robot/json/writer/RobotSnapshotJsonWriter.h"

namespace {
    const char *statusToString(CommandResponseStatus status) {
        switch (status) {
            case CommandResponseStatus::OK: return "ok";
            case CommandResponseStatus::ERROR: return "error";
            case CommandResponseStatus::UNKNOWN: return "unknown";
        }
        return "unknown";
    }

    const char *typeToString(CommandResponseType type) {
        switch (type) {
            case CommandResponseType::ACK: return "ack";
            case CommandResponseType::ERROR: return "error";
            case CommandResponseType::ROBOT_STATUS: return "robotStatus";
            case CommandResponseType::ROBOT_CONFIG: return "robotConfig";
            case CommandResponseType::MOTION_STATUS: return "motionStatus";
            case CommandResponseType::MOTION_CONFIG: return "motionConfig";
            case CommandResponseType::WIFI_CONFIG: return "wifiConfig";
            case CommandResponseType::WIFI_CREDENTIALS: return "wifiCredentials";
            case CommandResponseType::UNKNOWN: return "unknown";
        }
        return "unknown";
    }

    const char *errorToString(CommandError error) {
        switch (error) {
            case CommandError::NONE: return "none";
            case CommandError::INVALID_JSON: return "invalidJson";
            case CommandError::INVALID_ENVELOPE: return "invalidEnvelope";
            case CommandError::INVALID_COMMAND: return "invalidCommand";
            case CommandError::INVALID_PAYLOAD: return "invalidPayload";
            case CommandError::STORAGE_ERROR: return "storageError";
            case CommandError::APPLY_ERROR: return "applyError";
            case CommandError::SYSTEM_ERROR: return "systemError";
        }
        return "unknown";
    }

    bool writePayload(JsonObject payload, const CommandResponse &response) {
        switch (response.type) {
            case CommandResponseType::ROBOT_STATUS:
                RobotSnapshotJsonWriter::write(payload, response.payload.robot);
                return true;
            case CommandResponseType::ROBOT_CONFIG:
                RobotConfigJsonWriter::write(payload, response.payload.robotConfig);
                return true;
            case CommandResponseType::MOTION_STATUS:
                MotionSnapshotJsonWriter::write(payload, response.payload.motion);
                return true;
            case CommandResponseType::MOTION_CONFIG:
                MotionConfigJsonWriter::write(payload, response.payload.motionConfig);
                return true;
            case CommandResponseType::WIFI_CONFIG:
                WifiConfigJsonWriter::write(payload, response.payload.wifiConfig);
                return true;
            case CommandResponseType::WIFI_CREDENTIALS:
                WifiCredentialsJsonWriter::write(payload, response.payload.wifiCredentials);
                return true;
            case CommandResponseType::ACK:
            case CommandResponseType::ERROR:
                return true;
            case CommandResponseType::UNKNOWN:
                return false;
        }
        return false;
    }

    bool hasPayload(CommandResponseType type) {
        switch (type) {
            case CommandResponseType::ROBOT_STATUS:
            case CommandResponseType::ROBOT_CONFIG:
            case CommandResponseType::MOTION_STATUS:
            case CommandResponseType::MOTION_CONFIG:
            case CommandResponseType::WIFI_CONFIG:
            case CommandResponseType::WIFI_CREDENTIALS:
                return true;
            case CommandResponseType::ACK:
            case CommandResponseType::ERROR:
            case CommandResponseType::UNKNOWN:
                return false;
        }
        return false;
    }
}

namespace CommandJsonSerializer {
    bool serialize(const CommandResponse &response, char *buffer, size_t capacity) {
        if (buffer == nullptr || capacity == 0)
            return false;
        if (response.status == CommandResponseStatus::UNKNOWN ||
            response.type == CommandResponseType::UNKNOWN) {
            return false;
        }
        JsonDocument document;
        document["type"] = "res";
        document["id"] = response.id;
        document["status"] = statusToString(response.status);
        document["response"] = typeToString(response.type);
        document["error"] = errorToString(response.error);
        if (hasPayload(response.type)) {
            JsonObject payload = document["payload"].to<JsonObject>();
            if (!writePayload(payload, response))
                return false;
        }
        const size_t requiredCapacity = measureJson(document) + 1;
        return (requiredCapacity <= capacity)
            ? serializeJson(document, buffer, capacity) > 0
            : false;
    }
}
