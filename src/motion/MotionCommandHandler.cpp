#include "MotionCommandHandler.h"
#include "MotionDefaults.h"
#include "command/CommandResponseBuilder.h"

MotionCommandHandler::MotionCommandHandler(MotionController &motion, MotionStorage &storage)
    : _motion(motion),
      _storage(storage) {}

bool MotionCommandHandler::execute(const MotionCommand &command, CommandResponse &response) {
    switch (command.type) {
        case MotionCommandType::STATUS:
            CommandResponseBuilder::status(response, _motion.getSnapshot());
            return true;
        case MotionCommandType::GET_CONFIG: {
            MotionConfig cfg{};
            _motion.getConfig(cfg);
            CommandResponseBuilder::config(response, cfg);
            return true;
        }
        case MotionCommandType::SET_CONFIG:
            if (!_storage.saveConfig(command.payload.config)) {
                CommandResponseBuilder::error(response,CommandError::STORAGE_ERROR);
                return false;
            }
            _motion.setConfig(command.payload.config);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::RESET_CONFIG: {
            const MotionConfig cfg = MotionDefaults::config();
            if (!_storage.saveConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _motion.setConfig(cfg);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case MotionCommandType::RESET_ODOMETRY:
            _motion.resetOdometry();
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::STOP:
            _motion.stop();
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::BRAKE:
            _motion.brake();
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::DRIVE:
            _motion.drive(command.payload.drive.velocity, command.payload.drive.turn);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::DRIVE_FOR:
            _motion.driveFor(command.payload.driveFor.velocity, command.payload.driveFor.turn, command.payload.driveFor.durationMs);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::DRIVE_DISTANCE:
            _motion.driveDistance(command.payload.driveDistance.velocity, command.payload.driveDistance.distanceMeters);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::ROTATE_TO:
            _motion.rotateTo(command.payload.rotate.angleDegrees, command.payload.rotate.speed);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::ROTATE_BY:
            _motion.rotateBy(command.payload.rotate.angleDegrees, command.payload.rotate.speed);
            CommandResponseBuilder::ack(response);
            return true;
        case MotionCommandType::UNKNOWN:
            break;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
