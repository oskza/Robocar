#ifndef WIFI_COMMAND_HANDLER_H
#define WIFI_COMMAND_HANDLER_H
#include "command/CommandResponseBuilder.h"
#include "WifiCommand.h"
#include "robot/Robot.h"

class WifiCommandHandler {
public:
    static bool execute(
        Robot &robot,
        WifiCommand command,
        const WifiCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
