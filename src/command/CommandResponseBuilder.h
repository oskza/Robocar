#ifndef COMMAND_RESPONSE_BUILDER_H
#define COMMAND_RESPONSE_BUILDER_H
#include "CommandResponse.h"
#include "robot/RobotConfig.h"
#include "motion/MotionConfig.h"

class CommandResponseBuilder {
public:
    static void ack(CommandResponse &response, CommandPostAction postAction = CommandPostAction::NONE);
    static void error(CommandResponse &response, CommandError error);
    static void status(CommandResponse &response, const RobotSnapshot &snapshot);
    static void status(CommandResponse &response, const MotionSnapshot &snapshot);
    static void config(CommandResponse &response, const RobotConfig &config);
    static void config(CommandResponse &response, const MotionConfig &config);
    static void config(CommandResponse &response, const WifiConfig &config);
    static void credentials(CommandResponse &response, const WifiCredentials &credentials, bool accessPoint);
};
#endif
