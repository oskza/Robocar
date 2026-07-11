#include "MotionCommandHandler.h"

bool MotionCommandHandler::execute(
    Robot &robot,
    MotionCommand command,
    const MotionCommandPayload &payload,
    CommandResponse &response
) {
    switch (command) {
        case MotionCommand::STATUS:
            CommandResponseBuilder::status(response, robot.getMotionSnapshot());
            return true;
        case MotionCommand::GET_CONFIG: {
            MotionConfig cfg{};
            if (!robot.getMotionConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::config(response, cfg);
            return true;
        }
        case MotionCommand::SET_CONFIG:
            if (!robot.setMotionConfig(payload.cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::RESET_CONFIG:
            if (!robot.resetWifiConfig()) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::STOP:
            robot.stop();
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::DRIVE:
            robot.drive(payload.drive.velocity, payload.drive.turn);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::DRIVE_FOR:
            robot.driveFor(
                payload.driveFor.velocity,
                payload.driveFor.turn,
                payload.driveFor.durationMs
            );
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::DRIVE_DISTANCE:
            robot.driveDistance(
                payload.driveDistance.velocity,
                payload.driveDistance.distanceMeters
            );
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::ROTATE_TO:
            robot.rotateTo(
                payload.rotate.angleDegrees,
                payload.rotate.speed
            );
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommand::ROTATE_BY:
            robot.rotateBy(
                payload.rotate.angleDegrees,
                payload.rotate.speed
            );
            CommandResponseBuilder::ack(response);
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
