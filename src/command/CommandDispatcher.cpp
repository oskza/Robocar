#include "CommandDispatcher.h"
#include "CommandResponseBuilder.h"
#include "motion/MotionCommandHandler.h"
#include "network/WifiCommandHandler.h"
#include "robot/RobotCommandHandler.h"
#include "system/SystemCommandHandler.h"

CommandDispatcher::CommandDispatcher(
    Robot &robot,
    RobotStorage &robotStorage,
    MotionController &motion,
    MotionStorage &motionStorage,
    WifiController &wifi,
    WifiStorage &wifiStorage
)
    : _robot(robot),
      _robotStorage(robotStorage),
      _motion(motion),
      _motionStorage(motionStorage),
      _wifi(wifi),
      _wifiStorage(wifiStorage) {}

bool CommandDispatcher::dispatch(const CommandEnvelope &command, CommandResponse &response) {
    response.id = command.id;
    switch (command.domain) {
        case CommandDomain::ROBOT:
            return RobotCommandHandler::execute(_robot, _robotStorage, command.robot, response);
        case CommandDomain::SYSTEM:
            return SystemCommandHandler::execute(command.system, response);
        case CommandDomain::MOTION:
            return MotionCommandHandler::execute(_motion, _motionStorage, command.motion, response);
        case CommandDomain::WIFI:
            return WifiCommandHandler::execute(_wifi, _wifiStorage, command.wifi, response);
        case CommandDomain::UNKNOWN:
            break;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
