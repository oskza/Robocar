#ifndef JSON_VALUE_READER_H
#define JSON_VALUE_READER_H
#include <ArduinoJson.h>
#include <IPAddress.h>
#include <stddef.h>
#include <stdint.h>

namespace JsonValueReader {
    bool readBool(JsonVariantConst value, bool &out);
    bool readInt16(JsonVariantConst value, int16_t &out);
    bool readUint8(JsonVariantConst value, uint8_t &out);
    bool readUint32(JsonVariantConst value, uint32_t &out);
    bool readFloat(JsonVariantConst value, float &out);
    bool readCString(JsonVariantConst value, char *out, size_t outSize);
    bool readIpAddress(JsonVariantConst value, IPAddress &out);
}
#endif
