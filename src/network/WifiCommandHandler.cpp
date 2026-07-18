#include "WifiCommandHandler.h"
#include "WifiDefaults.h"
#include "command/CommandResponseBuilder.h"

namespace WifiCommandHandler {
    bool execute(
        WifiController &wifi,
        WifiStorage &storage,
        const WifiCommand &command,
        CommandResponse &response
    ) {
        switch (command.type) {
            case WifiCommandType::GET_CONFIG: {
                WifiConfig config{};
                wifi.getConfig(config);
                CommandResponseBuilder::config(response, config);
                return true;
            }
            case WifiCommandType::SET_CONFIG:
                if (!storage.saveConfig(command.payload.config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setConfig(command.payload.config);
                CommandResponseBuilder::ack(response);
                return true;
            case WifiCommandType::RESET_CONFIG: {
                const WifiConfig config = WifiDefaults::config();
                if (!storage.saveConfig(config)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setConfig(config);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case WifiCommandType::SET_STATION_CREDENTIALS:
                if (!storage.saveStationCredentials(command.payload.credentials)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setStationCredentials(command.payload.credentials);
                CommandResponseBuilder::ack(response);
                return true;
            case WifiCommandType::RESET_STATION_CREDENTIALS: {
                const WifiCredentials credentials = WifiDefaults::stationCredentials();
                if (!storage.saveStationCredentials(credentials)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setStationCredentials(credentials);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case WifiCommandType::GET_ACCESS_POINT_CREDENTIALS: {
                WifiCredentials credentials{};
                wifi.getAccessPointCredentials(credentials);
                CommandResponseBuilder::credentials(response, credentials);
                return true;
            }
            case WifiCommandType::SET_ACCESS_POINT_CREDENTIALS:
                if (!storage.saveAccessPointCredentials(command.payload.credentials)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setAccessPointCredentials(command.payload.credentials);
                CommandResponseBuilder::ack(response);
                return true;
            case WifiCommandType::RESET_ACCESS_POINT_CREDENTIALS: {
                const WifiCredentials credentials = WifiDefaults::accessPointCredentials();
                if (!storage.saveAccessPointCredentials(credentials)) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setAccessPointCredentials(credentials);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case WifiCommandType::RESET_ALL: {
                const WifiConfig config = WifiDefaults::config();
                const WifiCredentials stationCredentials = WifiDefaults::stationCredentials();
                const WifiCredentials accessPointCredentials = WifiDefaults::accessPointCredentials();
                bool saved = true;
                saved &= storage.saveConfig(config);
                saved &= storage.saveStationCredentials(stationCredentials);
                saved &= storage.saveAccessPointCredentials(accessPointCredentials);
                if (!saved) {
                    CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                    return false;
                }
                wifi.setConfig(config);
                wifi.setStationCredentials(stationCredentials);
                wifi.setAccessPointCredentials(accessPointCredentials);
                CommandResponseBuilder::ack(response);
                return true;
            }
            case WifiCommandType::UNKNOWN:
                break;
        }
        CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
        return false;
    }

}
