#include "MotionCommandJsonReader.h"

#include <string.h>
#include "../JsonValueReader.h"

bool MotionCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst commandValue = json["command"];

    if (!commandValue.is<const char *>())
        return false;

    const char *cmd = commandValue.as<const char *>();

    command.domain = CommandDomain::MOTION;

    JsonVariantConst payload = json["payload"];

    if (strcmp(cmd, "status") == 0) {
        command.command.motion = MotionCommand::STATUS;
        return true;
    }

    if (strcmp(cmd, "stop") == 0) {
        command.command.motion = MotionCommand::STOP;
        return true;
    }

    if (strcmp(cmd, "drive") == 0) {
        command.command.motion = MotionCommand::DRIVE;

        return JsonValueReader::readInt16(
                payload["velocity"],
                command.payload.motion.drive.velocity
            )
            && JsonValueReader::readInt16(
                payload["turn"],
                command.payload.motion.drive.turn
            );
    }

    if (strcmp(cmd, "driveFor") == 0) {
        command.command.motion = MotionCommand::DRIVE_FOR;

        return JsonValueReader::readInt16(
                payload["velocity"],
                command.payload.motion.driveFor.velocity
            )
            && JsonValueReader::readInt16(
                payload["turn"],
                command.payload.motion.driveFor.turn
            )
            && JsonValueReader::readUint32(
                payload["durationMs"],
                command.payload.motion.driveFor.durationMs
            );
    }

    if (strcmp(cmd, "driveDistance") == 0) {
        command.command.motion = MotionCommand::DRIVE_DISTANCE;

        return JsonValueReader::readInt16(
                payload["velocity"],
                command.payload.motion.driveDistance.velocity
            )
            && JsonValueReader::readFloat(
                payload["distanceMeters"],
                command.payload.motion.driveDistance.distanceMeters
            );
    }

    if (strcmp(cmd, "rotateTo") == 0) {
        command.command.motion = MotionCommand::ROTATE_TO;

        return JsonValueReader::readFloat(
                payload["angleDegrees"],
                command.payload.motion.rotate.angleDegrees
            )
            && JsonValueReader::readUint8(
                payload["speed"],
                command.payload.motion.rotate.speed
            );
    }

    if (strcmp(cmd, "rotateBy") == 0) {
        command.command.motion = MotionCommand::ROTATE_BY;

        return JsonValueReader::readFloat(
                payload["angleDegrees"],
                command.payload.motion.rotate.angleDegrees
            )
            && JsonValueReader::readUint8(
                payload["speed"],
                command.payload.motion.rotate.speed
            );
    }

    return false;
}
