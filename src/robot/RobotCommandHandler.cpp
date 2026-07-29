#include "RobotCommandHandler.h"
#include "RobotDefaults.h"
#include "command/CommandResponseBuilder.h"

RobotCommandHandler::RobotCommandHandler(Robot &robot, RobotStorage &storage)
    : _robot(robot),
      _storage(storage) {}

bool RobotCommandHandler::execute(const RobotCommand &command, CommandResponse &response) {
    switch (command.type) {
        case RobotCommandType::STATUS:
            CommandResponseBuilder::status(response, _robot.getSnapshot());
            return true;
        case RobotCommandType::GET_CONFIG: {
            RobotConfig cfg{};
            _robot.getConfig(cfg);
            CommandResponseBuilder::config(response, cfg);
            return true;
        }
        case RobotCommandType::SET_CONFIG:
            if (!_storage.saveConfig(command.payload.config)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _robot.setConfig(command.payload.config);
            CommandResponseBuilder::ack(response);
            return true;
        case RobotCommandType::RESET_CONFIG: {
            const RobotConfig cfg = RobotDefaults::config();
            if (!_storage.saveConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _robot.setConfig(cfg);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case RobotCommandType::ENABLE_TELEMETRY:
        case RobotCommandType::DISABLE_TELEMETRY: {
            RobotConfig cfg{};
            _robot.getConfig(cfg);
            const bool enabled = command.type == RobotCommandType::ENABLE_TELEMETRY;
            if (cfg.telemetryEnabled == enabled) {
                CommandResponseBuilder::ack(response);
                return true;
            }
            cfg.telemetryEnabled = enabled;
            if (!_storage.saveConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _robot.setConfig(cfg);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case RobotCommandType::UNKNOWN:
            break;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
