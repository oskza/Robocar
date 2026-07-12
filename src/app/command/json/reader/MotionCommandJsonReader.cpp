#include "MotionCommandJsonReader.h"
#include <string.h>
#include "../JsonValueReader.h"

bool MotionCommandJsonReader::_readConfig(JsonVariantConst &payload, MotionConfig &cfg) {
    return JsonValueReader::readFloat(payload["wheelDiameterMeters"], cfg.wheelDiameterMeters)
        && JsonValueReader::readFloat(payload["wheelCircumferenceFactor"], cfg.wheelCircumferenceFactor)
        && JsonValueReader::readUint8(payload["wheelAcceleration"], cfg.wheelAcceleration)
        && JsonValueReader::readUint8(payload["motorLeftMinPwm"], cfg.motorLeftMinPwm)
        && JsonValueReader::readUint8(payload["motorRightMinPwm"], cfg.motorRightMinPwm)
        && JsonValueReader::readFloat(payload["headingToleranceDegrees"], cfg.headingToleranceDegrees);
}

bool MotionCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst cmdVal = json["command"];
    if (!cmdVal.is<const char*>())
        return false;
    const char *cmdStr = cmdVal.as<const char*>();
    command.domain = CommandDomain::MOTION;
    if (strcmp(cmdStr, "status") == 0) {
        command.command.motion = MotionCommand::STATUS;
        return true;
    }
    if (strcmp(cmdStr, "getConfig") == 0) {
        command.command.motion = MotionCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(cmdStr, "setConfig") == 0) {
        command.command.motion = MotionCommand::SET_CONFIG;
        JsonVariantConst payload = json["payload"];
        return _readConfig(payload, command.payload.motion.cfg);
    }
    if (strcmp(cmdStr, "reset") == 0) {
        command.command.motion = MotionCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(cmdStr, "stop") == 0) {
        command.command.motion = MotionCommand::STOP;
        return true;
    }
    if (strcmp(cmdStr, "brake") == 0) {
        command.command.motion = MotionCommand::BRAKE;
        return true;
    }
    if (strcmp(cmdStr, "drive") == 0) {
        command.command.motion = MotionCommand::DRIVE;
        JsonVariantConst payload = json["payload"];
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.drive.velocity)
            && JsonValueReader::readInt16(payload["turn"], command.payload.motion.drive.turn);
    }
    if (strcmp(cmdStr, "driveFor") == 0) {
        command.command.motion = MotionCommand::DRIVE_FOR;
        JsonVariantConst payload = json["payload"];
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.driveFor.velocity)
            && JsonValueReader::readInt16(payload["turn"], command.payload.motion.driveFor.turn)
            && JsonValueReader::readUint32(payload["durationMs"], command.payload.motion.driveFor.durationMs);
    }
    if (strcmp(cmdStr, "driveDistance") == 0) {
        command.command.motion = MotionCommand::DRIVE_DISTANCE;
        JsonVariantConst payload = json["payload"];
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.driveDistance.velocity)
            && JsonValueReader::readFloat(payload["distanceMeters"], command.payload.motion.driveDistance.distanceMeters);
    }
    if (strcmp(cmdStr, "rotateTo") == 0) {
        command.command.motion = MotionCommand::ROTATE_TO;
        JsonVariantConst payload = json["payload"];
        return JsonValueReader::readFloat(payload["angleDegrees"], command.payload.motion.rotate.angleDegrees)
            && JsonValueReader::readUint8(payload["speed"], command.payload.motion.rotate.speed);
    }
    if (strcmp(cmdStr, "rotateBy") == 0) {
        command.command.motion = MotionCommand::ROTATE_BY;
        JsonVariantConst payload = json["payload"];
        return JsonValueReader::readFloat(payload["angleDegrees"], command.payload.motion.rotate.angleDegrees)
            && JsonValueReader::readUint8(payload["speed"], command.payload.motion.rotate.speed);
    }
    return false;
}
