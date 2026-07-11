#include "CommandDispatcher.h"
#include "CommandResponseBuilder.h"
#include "handler/RobotCommandHandler.h"
#include "handler/SystemCommandHandler.h"
#include "handler/PowerCommandHandler.h"
#include "handler/MotionCommandHandler.h"
#include "handler/WifiCommandHandler.h"

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
                _robot,
                command.command.system,
                command.payload.system,
                response
            );

        case CommandDomain::POWER:
            return PowerCommandHandler::execute(
                _robot,
                command.command.power,
                command.payload.power,
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
