#include "CommandResponseBuilder.h"

void CommandResponseBuilder::ack(CommandResponse &response) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::ACK;
    response.error = CommandError::NONE;
}

void CommandResponseBuilder::error(CommandResponse &response, CommandError error) {
    response.status = CommandResponseStatus::ERROR;
    response.type = CommandResponseType::ERROR;
    response.error = error;
}

void CommandResponseBuilder::status(CommandResponse &response, const RobotSnapshot &snapshot) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::ROBOT_STATUS;
    response.error = CommandError::NONE;
    response.payload.robot = snapshot;
}

void CommandResponseBuilder::status(CommandResponse &response, const SystemSnapshot &snapshot) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::SYSTEM_STATUS;
    response.error = CommandError::NONE;
    response.payload.system = snapshot;
}

void CommandResponseBuilder::status(CommandResponse &response, const MotionSnapshot &snapshot) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::MOTION_STATUS;
    response.error = CommandError::NONE;
    response.payload.motion = snapshot;
}

void CommandResponseBuilder::status(CommandResponse &response, const WifiSnapshot &snapshot) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::WIFI_STATUS;
    response.error = CommandError::NONE;
    response.payload.wifi = snapshot;
}

void CommandResponseBuilder::config(CommandResponse &response, const RobotConfig &config) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::ROBOT_CONFIG;
    response.error = CommandError::NONE;
    response.payload.robotConfig = config;
}

void CommandResponseBuilder::config(CommandResponse &response, const MotionConfig &config) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::MOTION_CONFIG;
    response.error = CommandError::NONE;
    response.payload.motionConfig = config;
}

void CommandResponseBuilder::config(CommandResponse &response, const WifiConfig &config) {
    response.status = CommandResponseStatus::OK;
    response.type = CommandResponseType::WIFI_CONFIG;
    response.error = CommandError::NONE;
    response.payload.wifiConfig = config;
}

void CommandResponseBuilder::credentials(CommandResponse &response, const WifiCredentials &credentials, bool accessPoint) {
    response.status = CommandResponseStatus::OK;
    response.type = (accessPoint)
        ? CommandResponseType::WIFI_ACCESS_POINT_CREDENTIALS
        : CommandResponseType::WIFI_STATION_CREDENTIALS;
    response.error = CommandError::NONE;
    response.payload.wifiCredentials = credentials;
}
