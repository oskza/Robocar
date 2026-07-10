#ifndef JSON_VALUE_READER_H
#define JSON_VALUE_READER_H
#include <ArduinoJson.h>
#include <stddef.h>
#include <IPAddress.h>

class JsonValueReader {
public:
    static bool readBool(JsonVariantConst value, bool &out);
    static bool readInt16(JsonVariantConst value, int16_t &out);
    static bool readUint8(JsonVariantConst value, uint8_t &out);
    static bool readUint32(JsonVariantConst value, uint32_t &out);
    static bool readFloat(JsonVariantConst value, float &out);
    static bool readCString(JsonVariantConst value, char *out, size_t outSize);
    static bool readIpAddress(JsonVariantConst value, IPAddress &out);
};
#endif
