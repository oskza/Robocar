#include "CommandResponseBuilder.h"

namespace {
    void success(CommandResponse &response, CommandResponseType type) {
        response.status = CommandResponseStatus::OK;
        response.type = type;
        response.error = CommandError::NONE;
        response.postAction = CommandPostAction::NONE;
    }
}

namespace CommandResponseBuilder {
    void ack(CommandResponse &response, CommandPostAction postAction) {
        success(response, CommandResponseType::ACK);
        response.postAction = postAction;
    }

    void error(CommandResponse &response, CommandError error) {
        response.status = CommandResponseStatus::ERROR;
        response.type = CommandResponseType::ERROR;
        response.error = error;
        response.postAction = CommandPostAction::NONE;
    }

    void status(CommandResponse &response, const RobotSnapshot &snapshot) {
        success(response, CommandResponseType::ROBOT_STATUS);
        response.payload.robot = snapshot;
    }

    void status(CommandResponse &response, const MotionSnapshot &snapshot) {
        success(response, CommandResponseType::MOTION_STATUS);
        response.payload.motion = snapshot;
    }

    void config(CommandResponse &response, const RobotConfig &config) {
        success(response, CommandResponseType::ROBOT_CONFIG);
        response.payload.robotConfig = config;
    }

    void config(CommandResponse &response, const MotionConfig &config) {
        success(response, CommandResponseType::MOTION_CONFIG);
        response.payload.motionConfig = config;
    }

    void config(CommandResponse &response, const WifiConfig &config) {
        success(response, CommandResponseType::WIFI_CONFIG);
        response.payload.wifiConfig = config;
    }

    void credentials(CommandResponse &response, const WifiCredentials &credentials) {
        success(response, CommandResponseType::WIFI_CREDENTIALS);
        response.payload.wifiCredentials = credentials;
    }
}
