#include "JsonValueReader.h"
#include <string.h>
#include <limits.h>

bool JsonValueReader::readBool(JsonVariantConst value, bool &out) {
    if (!value.is<bool>())
        return false;
    out = value.as<bool>();
    return true;
}

bool JsonValueReader::readInt16(JsonVariantConst value, int16_t &out) {
    if (!value.is<int>())
        return false;
    const int v = value.as<int>();
    if (v < INT16_MIN || v > INT16_MAX)
        return false;
    out = (int16_t)v;
    return true;
}

bool JsonValueReader::readUint8(JsonVariantConst value, uint8_t &out) {
    if (!value.is<int>())
        return false;
    const int v = value.as<int>();
    if (v < 0 || v > UINT8_MAX)
        return false;
    out = (uint8_t)v;
    return true;
}

bool JsonValueReader::readUint32(JsonVariantConst value, uint32_t &out) {
    if (!value.is<uint32_t>())
        return false;
    out = value.as<uint32_t>();
    return true;
}

bool JsonValueReader::readFloat(JsonVariantConst value, float &out) {
    if (!value.is<float>() && !value.is<int>())
        return false;
    out = value.as<float>();
    return true;
}

bool JsonValueReader::readCString(
    JsonVariantConst value,
    char *out,
    size_t outSize
) {
    if (!out || outSize == 0)
        return false;
    if (!value.is<const char *>())
        return false;
    const char *src = value.as<const char *>();
    if (!src)
        return false;
    strncpy(out, src, outSize);
    out[outSize - 1] = '\0';
    return true;
}

bool JsonValueReader::readIpAddress(JsonVariantConst value, IPAddress &out) {
    if (!value.is<const char *>())
        return false;
    const char *text = value.as<const char *>();
    if (!text)
        return false;
    return out.fromString(text);
}
