#ifndef WIFI_COMMAND_HANDLER_H
#define WIFI_COMMAND_HANDLER_H
#include "WifiCommand.h"
#include "WifiController.h"
#include "WifiStorage.h"
#include "command/CommandResponse.h"

namespace WifiCommandHandler {
    bool execute(
        WifiController &wifi,
        WifiStorage &storage,
        const WifiCommand &command,
        CommandResponse &response
    );
}
#endif
