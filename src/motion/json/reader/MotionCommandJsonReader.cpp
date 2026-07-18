#include "MotionCommandJsonReader.h"
#include <string.h>
#include <JsonValueReader.h>

namespace {
    bool readConfig(JsonObjectConst json, MotionConfig &config) {
        return JsonValueReader::readFloat(json["wheelDiameterMeters"], config.wheelDiameterMeters)
            && JsonValueReader::readFloat(json["wheelCircumferenceFactor"], config.wheelCircumferenceFactor)
            && JsonValueReader::readUint8(json["wheelAcceleration"], config.wheelAcceleration)
            && JsonValueReader::readUint8(json["motorLeftMinPwm"], config.motorLeftMinPwm)
            && JsonValueReader::readUint8(json["motorRightMinPwm"], config.motorRightMinPwm)
            && JsonValueReader::readFloat(json["headingToleranceDegrees"], config.headingToleranceDegrees);
    }

    bool readDrive(JsonObjectConst json, MotionDrivePayload &payload) {
        return JsonValueReader::readInt16(json["velocity"], payload.velocity)
            && JsonValueReader::readInt16(json["turn"], payload.turn);
    }

    bool readDriveFor(JsonObjectConst json, MotionDriveForPayload &payload) {
        return JsonValueReader::readInt16(json["velocity"], payload.velocity)
            && JsonValueReader::readInt16(json["turn"], payload.turn)
            && JsonValueReader::readUint32(json["durationMs"], payload.durationMs);
    }

    bool readDriveDistance(JsonObjectConst json, MotionDriveDistancePayload &payload) {
        return JsonValueReader::readInt16(json["velocity"], payload.velocity)
            && JsonValueReader::readFloat(json["distanceMeters"], payload.distanceMeters);
    }

    bool readRotate(JsonObjectConst json, MotionRotatePayload &payload) {
        return JsonValueReader::readFloat(json["angleDegrees"], payload.angleDegrees)
            && JsonValueReader::readUint8(json["speed"], payload.speed);
    }
}

namespace MotionCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, MotionCommand &out) {
        if (command == nullptr)
            return false;

        out = {};

        if (strcmp(command, "status") == 0) {
            out.type = MotionCommandType::STATUS;
            return true;
        }

        if (strcmp(command, "getConfig") == 0) {
            out.type = MotionCommandType::GET_CONFIG;
            return true;
        }

        if (strcmp(command, "setConfig") == 0) {
            out.type = MotionCommandType::SET_CONFIG;
            return readConfig(payload, out.payload.config);
        }

        if (strcmp(command, "resetConfig") == 0) {
            out.type = MotionCommandType::RESET_CONFIG;
            return true;
        }

        if (strcmp(command, "resetOdometry") == 0) {
            out.type = MotionCommandType::RESET_ODOMETRY;
            return true;
        }

        if (strcmp(command, "stop") == 0) {
            out.type = MotionCommandType::STOP;
            return true;
        }

        if (strcmp(command, "brake") == 0) {
            out.type = MotionCommandType::BRAKE;
            return true;
        }

        if (strcmp(command, "drive") == 0) {
            out.type = MotionCommandType::DRIVE;
            return readDrive(payload, out.payload.drive);
        }

        if (strcmp(command, "driveFor") == 0) {
            out.type = MotionCommandType::DRIVE_FOR;
            return readDriveFor(payload, out.payload.driveFor);
        }

        if (strcmp(command, "driveDistance") == 0) {
            out.type = MotionCommandType::DRIVE_DISTANCE;
            return readDriveDistance(payload, out.payload.driveDistance);
        }

        if (strcmp(command, "rotateTo") == 0) {
            out.type = MotionCommandType::ROTATE_TO;
            return readRotate(payload, out.payload.rotate);
        }

        if (strcmp(command, "rotateBy") == 0) {
            out.type = MotionCommandType::ROTATE_BY;
            return readRotate(payload, out.payload.rotate);
        }

        return false;
    }
}
