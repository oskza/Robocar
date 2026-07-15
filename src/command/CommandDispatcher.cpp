#include "CommandDispatcher.h"
#include "CommandResponseBuilder.h"
#include "robot/RobotCommandHandler.h"
#include "system/SystemCommandHandler.h"
#include "motion/MotionCommandHandler.h"
#include "network/WifiCommandHandler.h"

CommandDispatcher::CommandDispatcher(Robot &robot) : _robot(robot) {}

bool CommandDispatcher::dispatch(const CommandEnvelope &command, CommandResponse &response) {
    response.id = command.id;
    switch (command.domain) {
        case CommandDomain::ROBOT:
            return RobotCommandHandler::execute(
                _robot,
                command.command.robot,
                command.payload.robot,
                response
            );

        case CommandDomain::SYSTEM:
            return SystemCommandHandler::execute(
                command.command.system,
                command.payload.system,
                response
            );

        case CommandDomain::MOTION:
            return MotionCommandHandler::execute(
                _robot,
                command.command.motion,
                command.payload.motion,
                response
            );

        case CommandDomain::WIFI:
            return WifiCommandHandler::execute(
                _robot,
                command.command.wifi,
                command.payload.wifi,
                response
            );
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
