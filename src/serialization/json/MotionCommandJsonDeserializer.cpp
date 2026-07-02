#include "MotionCommandJsonDeserializer.h"
#include <string.h>

bool MotionCommandJsonDeserializer::_readInt16(JsonVariantConst value, int16_t &out) {
    if (!value.is<int>())
        return false;
    int v = value.as<int>();
    if (v < INT16_MIN || v > INT16_MAX)
        return false;
    out = (int16_t)v;
    return true;
}

bool MotionCommandJsonDeserializer::_readUint32(JsonVariantConst value, uint32_t &out) {
    if (!value.is<uint32_t>())
        return false;
    out = value.as<uint32_t>();
    return true;
}

bool MotionCommandJsonDeserializer::_readUint8(JsonVariantConst value, uint8_t &out) {
    if (!value.is<int>())
        return false;
    int v = value.as<int>();
    if (v < 0 || v > UINT8_MAX)
        return false;
    out = (uint8_t)v;
    return true;
}

bool MotionCommandJsonDeserializer::_readFloat(JsonVariantConst value, float &out) {
    if (!value.is<float>() && !value.is<int>())
        return false;
    out = value.as<float>();
    return true;
}

MotionCommandReadResult MotionCommandJsonDeserializer::read(const char *data, size_t len) {
    if (!data || len == 0)
        return MotionCommandReadResult::failure();
    JsonDocument doc;

    if (deserializeJson(doc, data, len))
        return MotionCommandReadResult::failure();

    JsonVariantConst cmdValue = doc["cmd"];

    if (!cmdValue.is<const char *>())
        return MotionCommandReadResult::failure();

    const char *cmd = cmdValue.as<const char *>();

    if (strcmp(cmd, "stop") == 0) {
        return MotionCommandReadResult::success(MotionCommand::stop());
    }

    if (strcmp(cmd, "drive") == 0) {
        int16_t velocity;
        int16_t turn;

        if (!_readInt16(doc["velocity"], velocity))
            return MotionCommandReadResult::failure();

        if (!_readInt16(doc["turn"], turn))
            return MotionCommandReadResult::failure();

        return MotionCommandReadResult::success(
            MotionCommand::drive(velocity, turn)
        );
    }

    if (strcmp(cmd, "driveFor") == 0) {
        int16_t velocity;
        int16_t turn;
        uint32_t durationMs;

        if (!_readInt16(doc["velocity"], velocity))
            return MotionCommandReadResult::failure();

        if (!_readInt16(doc["turn"], turn))
            return MotionCommandReadResult::failure();

        if (!_readUint32(doc["durationMs"], durationMs))
            return MotionCommandReadResult::failure();

        return MotionCommandReadResult::success(
            MotionCommand::driveFor(velocity, turn, durationMs)
        );
    }

    if (strcmp(cmd, "driveDistance") == 0) {
        int16_t velocity;
        float distanceMeters;

        if (!_readInt16(doc["velocity"], velocity))
            return MotionCommandReadResult::failure();

        if (!_readFloat(doc["distanceMeters"], distanceMeters))
            return MotionCommandReadResult::failure();

        return MotionCommandReadResult::success(
            MotionCommand::driveDistance(velocity, distanceMeters)
        );
    }

    if (strcmp(cmd, "rotateTo") == 0) {
        float angleDegrees;
        uint8_t speed;

        if (!_readFloat(doc["angleDegrees"], angleDegrees))
            return MotionCommandReadResult::failure();

        if (!_readUint8(doc["speed"], speed))
            return MotionCommandReadResult::failure();

        return MotionCommandReadResult::success(
            MotionCommand::rotateTo(angleDegrees, speed)
        );
    }

    if (strcmp(cmd, "rotateBy") == 0) {
        float angleDegrees;
        uint8_t speed;

        if (!_readFloat(doc["angleDegrees"], angleDegrees))
            return MotionCommandReadResult::failure();

        if (!_readUint8(doc["speed"], speed))
            return MotionCommandReadResult::failure();

        return MotionCommandReadResult::success(
            MotionCommand::rotateBy(angleDegrees, speed)
        );
    }

    return MotionCommandReadResult::failure();
}
