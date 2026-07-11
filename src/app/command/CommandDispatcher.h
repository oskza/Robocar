#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H
#include "common/CommandEnvelope.h"
#include "common/CommandResponse.h"
#include "../../robot/Robot.h"

class CommandDispatcher {
private:
    Robot &_robot;
public:
    explicit CommandDispatcher(Robot &robot);
    bool dispatch(const CommandEnvelope &command, CommandResponse &response);
};
#endif
