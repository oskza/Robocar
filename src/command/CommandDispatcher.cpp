#include "CommandDispatcher.h"
#include "CommandResponseBuilder.h"
#include "system/SystemCommandHandler.h"

CommandDispatcher::CommandDispatcher(
    RobotCommandHandler &robot,
    MotionCommandHandler &motion,
    WifiCommandHandler &wifi,
    SystemCommandHandler &system
)
    : _robot(robot),
        _motion(motion),
        _wifi(wifi),
        _system(system) {}

bool CommandDispatcher::dispatch(const CommandEnvelope &command, CommandResponse &response) {
    response.id = command.id;
    switch (command.domain) {
        case CommandDomain::ROBOT: return _robot.execute(command.robot, response);
        case CommandDomain::SYSTEM: return _system.execute(command.system, response);
        case CommandDomain::WIFI: return _wifi.execute(command.wifi, response);
        case CommandDomain::MOTION: return _motion.execute(command.motion, response);
        case CommandDomain::UNKNOWN: break;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
