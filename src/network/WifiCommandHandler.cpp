#include "WifiCommandHandler.h"

bool WifiCommandHandler::execute(
    Robot &robot,
    WifiCommand command,
    const WifiCommandPayload &payload,
    CommandResponse &response
) {
    switch (command) {
        case WifiCommand::GET_CONFIG: {
            WifiConfig cfg{};
            robot.getWifiConfig(cfg);
            CommandResponseBuilder::config(response, cfg);
            return true;
        }

        case WifiCommand::SET_CONFIG:
            if (!robot.setWifiConfig(payload.cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::RESET_CONFIG:
            robot.resetWifiConfig();
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::SET_STATION_CREDENTIALS:
            if (!robot.setStationCredentials(payload.credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::RESET_STATION_CREDENTIALS:
            robot.resetStationCredentials();
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::GET_ACCESS_POINT_CREDENTIALS: {
            WifiCredentials credentials{};
            robot.getAccessPointCredentials(credentials);
            CommandResponseBuilder::credentials(response, credentials, true);
            return true;
        }

        case WifiCommand::SET_ACCESS_POINT_CREDENTIALS:
            if (!robot.setAccessPointCredentials(payload.credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::RESET_ACCESS_POINT_CREDENTIALS:
            robot.resetAccessPointCredentials();
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::RESET_ALL:
            robot.resetWifi();
            CommandResponseBuilder::ack(response);
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
