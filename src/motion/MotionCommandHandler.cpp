#include "MotionCommandHandler.h"
#include "MotionDefaults.h"
#include "command/CommandResponseBuilder.h"

namespace MotionCommandHandler {
    bool execute(
        MotionController &motion,
        MotionStorage &storage,
        const MotionCommand &command,
        CommandResponse &response
    ) {
        switch (command.type) {
            case MotionCommandType::STATUS:
                CommandResponseBuilder::status(response, motion.getSnapshot());
                return true;

            case MotionCommandType::GET_CONFIG: {
                MotionConfig config{};
                motion.getConfig(config);
                CommandResponseBuilder::config(response, config);
                return true;
            }

            case MotionCommandType::SET_CONFIG:
                if (!storage.saveConfig(command.payload.config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                motion.setConfig(command.payload.config);
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::RESET_CONFIG: {
                const MotionConfig config = MotionDefaults::config();
                if (!storage.saveConfig(config)) {
                    CommandResponseBuilder::error(
                        response,
                        CommandError::STORAGE_ERROR
                    );
                    return false;
                }
                motion.setConfig(config);
                CommandResponseBuilder::ack(response);
                return true;
            }

            case MotionCommandType::RESET_ODOMETRY:
                motion.resetOdometry();
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::STOP:
                motion.stop();
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::BRAKE:
                motion.brake();
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::DRIVE:
                motion.drive(command.payload.drive.velocity, command.payload.drive.turn);
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::DRIVE_FOR:
                motion.driveFor(
                    command.payload.driveFor.velocity,
                    command.payload.driveFor.turn,
                    command.payload.driveFor.durationMs
                );
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::DRIVE_DISTANCE:
                motion.driveDistance(command.payload.driveDistance.velocity, command.payload.driveDistance.distanceMeters);
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::ROTATE_TO:
                motion.rotateTo(command.payload.rotate.angleDegrees, command.payload.rotate.speed);
                CommandResponseBuilder::ack(response);
                return true;

            case MotionCommandType::ROTATE_BY:
                motion.rotateBy(command.payload.rotate.angleDegrees, command.payload.rotate.speed);
                CommandResponseBuilder::ack(response);
                return true;
            case MotionCommandType::UNKNOWN:
                break;
        }
        CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
        return false;
    }
}
