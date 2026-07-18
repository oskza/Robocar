#ifndef COMMAND_RESPONSE_BUILDER_H
#define COMMAND_RESPONSE_BUILDER_H
#include "CommandResponse.h"

namespace CommandResponseBuilder {
    void ack(CommandResponse &response, CommandPostAction postAction = CommandPostAction::NONE);
    void error(CommandResponse &response, CommandError error);
    void status(CommandResponse &response, const RobotSnapshot &snapshot);
    void status(CommandResponse &response, const MotionSnapshot &snapshot);
    void config(CommandResponse &response, const RobotConfig &config);
    void config(CommandResponse &response, const MotionConfig &config);
    void config(CommandResponse &response, const WifiConfig &config);
    void credentials(CommandResponse &response, const WifiCredentials &credentials);
}
#endif
