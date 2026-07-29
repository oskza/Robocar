#include "WifiCommandHandler.h"
#include "WifiDefaults.h"
#include "command/CommandResponseBuilder.h"

WifiCommandHandler::WifiCommandHandler(WifiController &wifi, WifiStorage &storage)
    : _wifi(wifi),
      _storage(storage) {}

bool WifiCommandHandler::execute(const WifiCommand &command, CommandResponse &response) {
    switch (command.type) {
        case WifiCommandType::GET_CONFIG: {
            WifiConfig cfg{};
            _wifi.getConfig(cfg);
            CommandResponseBuilder::config(response, cfg);
            return true;
        }
        case WifiCommandType::SET_CONFIG:
            if (!_storage.saveConfig(command.payload.config)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setConfig(command.payload.config);
            CommandResponseBuilder::ack(response);
            return true;
        case WifiCommandType::RESET_CONFIG: {
            const WifiConfig cfg = WifiDefaults::config();
            if (!_storage.saveConfig(cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setConfig(cfg);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case WifiCommandType::SET_STATION_CREDENTIALS:
            if (!_storage.saveStationCredentials(command.payload.credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setStationCredentials(command.payload.credentials);
            CommandResponseBuilder::ack(response);
            return true;
        case WifiCommandType::RESET_STATION_CREDENTIALS: {
            const WifiCredentials credentials = WifiDefaults::stationCredentials();
            if (!_storage.saveStationCredentials(credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setStationCredentials(credentials);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case WifiCommandType::GET_ACCESS_POINT_CREDENTIALS: {
            WifiCredentials credentials{};
            _wifi.getAccessPointCredentials(credentials);
            CommandResponseBuilder::credentials(response, credentials);
            return true;
        }
        case WifiCommandType::SET_ACCESS_POINT_CREDENTIALS:
            if (!_storage.saveAccessPointCredentials(command.payload.credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setAccessPointCredentials(command.payload.credentials);
            CommandResponseBuilder::ack(response);
            return true;
        case WifiCommandType::RESET_ACCESS_POINT_CREDENTIALS: {
            const WifiCredentials credentials = WifiDefaults::accessPointCredentials();
            if (!_storage.saveAccessPointCredentials(credentials)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setAccessPointCredentials(credentials);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case WifiCommandType::RESET_ALL: {
            const WifiConfig cfg = WifiDefaults::config();
            const WifiCredentials stationCredentials = WifiDefaults::stationCredentials();
            const WifiCredentials accessPointCredentials = WifiDefaults::accessPointCredentials();
            bool saved = true;
            saved &= _storage.saveConfig(cfg);
            saved &= _storage.saveStationCredentials(stationCredentials);
            saved &= _storage.saveAccessPointCredentials(accessPointCredentials);
            if (!saved) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            _wifi.setConfig(cfg);
            _wifi.setStationCredentials(stationCredentials);
            _wifi.setAccessPointCredentials(accessPointCredentials);
            CommandResponseBuilder::ack(response);
            return true;
        }
        case WifiCommandType::UNKNOWN:
            break;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
