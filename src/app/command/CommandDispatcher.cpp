#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(
    RobotCommandHandler &robot,
    SystemCommandHandler &system,
    MotionCommandHandler &motion,
    WifiCommandHandler &wifi
)
    : _robot(robot),
      _system(system),
      _motion(motion),
      _wifi(wifi) {}

bool CommandDispatcher::dispatch(
    const CommandEnvelope &command,
    CommandResponse &response
) {
    response.id = command.id;

    switch (command.domain) {
        case CommandDomain::ROBOT:
            return _robot.execute(
                command.command.robot,
                command.payload.robot,
                response
            );

        case CommandDomain::SYSTEM:
            return _system.execute(
                command.command.system,
                command.payload.system,
                response
            );

        case CommandDomain::MOTION:
            return _motion.execute(
                command.command.motion,
                command.payload.motion,
                response
            );

        case CommandDomain::WIFI:
            return _wifi.execute(
                command.command.wifi,
                command.payload.wifi,
                response
            );
    }

    response.status = CommandResponseStatus::ERROR;
    response.type = CommandResponseType::ERROR;
    response.error = CommandError::INVALID_COMMAND;

    return false;
}
