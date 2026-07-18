#include "RobotCommandHandler.h"
#include "RobotDefaults.h"
#include "command/CommandResponseBuilder.h"

namespace RobotCommandHandler {
    bool execute(Robot &robot, RobotStorage &storage, const RobotCommand &command, CommandResponse &response) {
        switch (command.type) {
            case RobotCommandType::STATUS:
                CommandResponseBuilder::status(response, robot.getSnapshot());
                return true;
            case RobotCommandType::GET_CONFIG: {
                RobotConfig config{};
                robot.getConfig(config);
                CommandResponseBuilder::config(response, config);
                return true;
            }
            case RobotCommandType::SET_CONFIG:
                if (!storage.saveConfig(command.payload.config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                robot.setConfig(command.payload.config);
                CommandResponseBuilder::ack(response);
                return true;
            case RobotCommandType::RESET_CONFIG: {
                const RobotConfig config = RobotDefaults::config();
                if (!storage.saveConfig(config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                robot.setConfig(config);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case RobotCommandType::ENABLE_TELEMETRY:
            case RobotCommandType::DISABLE_TELEMETRY: {
                RobotConfig config{};
                robot.getConfig(config);
                const bool enable = command.type == RobotCommandType::ENABLE_TELEMETRY;
                if (config.telemetryEnabled == enable) {
                    CommandResponseBuilder::ack(response);
                    return true;
                }
                config.telemetryEnabled = enable;
                if (!storage.saveConfig(config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                robot.setConfig(config);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case RobotCommandType::UNKNOWN:
                break;
        }
        CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
        return false;
    }
}
