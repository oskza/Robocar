#include "WifiCommandHandler.h"

bool WifiCommandHandler::execute(
    Robot &robot,
    WifiCommand command,
    const WifiCommandPayload &payload,
    CommandResponse &response
) {
    switch (command) {
        case WifiCommand::STATUS:
            CommandResponseBuilder::status(response, robot.getWifiSnapshot());
            return true;

        case WifiCommand::GET_CONFIG: {
            WifiConfig cfg{};
            if (!robot.getWifiConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
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
            if (!robot.resetWifiConfig()) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
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
            if (!robot.resetStationCredentials()) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::GET_ACCESS_POINT_CREDENTIALS: {
            WifiCredentials credentials{};
            if (!robot.getAccessPointCredentials(credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
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
            if (!robot.resetAccessPointCredentials()) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;

        case WifiCommand::RESET_ALL:
            if (!robot.resetWifi()) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
