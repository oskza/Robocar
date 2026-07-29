#ifndef WIFI_COMMAND_HANDLER_H
#define WIFI_COMMAND_HANDLER_H
#include "WifiCommand.h"
#include "WifiController.h"
#include "WifiStorage.h"
#include "command/CommandResponse.h"

class WifiCommandHandler {
private:
    WifiController &_wifi;
    WifiStorage &_storage;
public:
    WifiCommandHandler(WifiController &wifi, WifiStorage &storage);
    bool execute(const WifiCommand &command, CommandResponse &response);
};
#endif
