#include "MotionCommandJsonReader.h"
#include <string.h>
#include "json/JsonValueReader.h"

bool MotionCommandJsonReader::_readConfig(JsonObjectConst json, MotionConfig &cfg) {
    return JsonValueReader::readFloat(json["wheelDiameterMeters"], cfg.wheelDiameterMeters)
        && JsonValueReader::readFloat(json["wheelCircumferenceFactor"], cfg.wheelCircumferenceFactor)
        && JsonValueReader::readUint8(json["wheelAcceleration"], cfg.wheelAcceleration)
        && JsonValueReader::readUint8(json["motorLeftMinPwm"], cfg.motorLeftMinPwm)
        && JsonValueReader::readUint8(json["motorRightMinPwm"], cfg.motorRightMinPwm)
        && JsonValueReader::readFloat(json["headingToleranceDegrees"], cfg.headingToleranceDegrees);
}

bool MotionCommandJsonReader::read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command) {
    if (commandName == nullptr)
        return false;
    command.domain = CommandDomain::MOTION;
    if (strcmp(commandName, "status") == 0) {
        command.command.motion = MotionCommand::STATUS;
        return true;
    }
    if (strcmp(commandName, "getConfig") == 0) {
        command.command.motion = MotionCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "setConfig") == 0) {
        command.command.motion = MotionCommand::SET_CONFIG;
        return _readConfig(payload, command.payload.motion.cfg);
    }
    if (strcmp(commandName, "reset") == 0) {
        command.command.motion = MotionCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "stop") == 0) {
        command.command.motion = MotionCommand::STOP;
        return true;
    }
    if (strcmp(commandName, "brake") == 0) {
        command.command.motion = MotionCommand::BRAKE;
        return true;
    }
    if (strcmp(commandName, "drive") == 0) {
        command.command.motion = MotionCommand::DRIVE;
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.drive.velocity)
            && JsonValueReader::readInt16(payload["turn"], command.payload.motion.drive.turn);
    }
    if (strcmp(commandName, "driveFor") == 0) {
        command.command.motion = MotionCommand::DRIVE_FOR;
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.driveFor.velocity)
            && JsonValueReader::readInt16(payload["turn"], command.payload.motion.driveFor.turn)
            && JsonValueReader::readUint32(payload["durationMs"], command.payload.motion.driveFor.durationMs);
    }
    if (strcmp(commandName, "driveDistance") == 0) {
        command.command.motion = MotionCommand::DRIVE_DISTANCE;
        return JsonValueReader::readInt16(payload["velocity"], command.payload.motion.driveDistance.velocity)
            && JsonValueReader::readFloat(payload["distanceMeters"], command.payload.motion.driveDistance.distanceMeters);
    }
    if (strcmp(commandName, "rotateTo") == 0) {
        command.command.motion = MotionCommand::ROTATE_TO;
        return JsonValueReader::readFloat(payload["angleDegrees"], command.payload.motion.rotate.angleDegrees)
            && JsonValueReader::readUint8(payload["speed"], command.payload.motion.rotate.speed);
    }
    if (strcmp(commandName, "rotateBy") == 0) {
        command.command.motion = MotionCommand::ROTATE_BY;
        return JsonValueReader::readFloat(payload["angleDegrees"], command.payload.motion.rotate.angleDegrees)
            && JsonValueReader::readUint8(payload["speed"], command.payload.motion.rotate.speed);
    }
    return false;
}
