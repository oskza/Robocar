#ifndef MOTION_COMMAND_JSON_DESERIALIZER_H
#define MOTION_COMMAND_JSON_DESERIALIZER_H
#include <ArduinoJson.h>
#include "MotionCommandReadResult.h"

class MotionCommandJsonDeserializer {
private:
    static bool _readInt16(JsonVariantConst value, int16_t &out);
    static bool _readUint32(JsonVariantConst value, uint32_t &out);
    static bool _readUint8(JsonVariantConst value, uint8_t &out);
    static bool _readFloat(JsonVariantConst value, float &out);
public:
    static MotionCommandReadResult read(const char *data, size_t len);
};
#endif
