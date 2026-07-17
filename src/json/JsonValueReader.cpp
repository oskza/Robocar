#include "JsonValueReader.h"
#include <limits.h>
#include <string.h>

namespace JsonValueReader {
    bool readBool(JsonVariantConst value, bool &out) {
        if (!value.is<bool>())
            return false;
        out = value.as<bool>();
        return true;
    }

    bool readInt16(JsonVariantConst value, int16_t &out) {
        if (!value.is<int32_t>())
            return false;
        const int32_t parsed = value.as<int32_t>();
        if (parsed < INT16_MIN || parsed > INT16_MAX)
            return false;
        out = (int16_t)parsed;
        return true;
    }

    bool readUint8(JsonVariantConst value, uint8_t &out) {
        if (!value.is<int32_t>())
            return false;
        const int32_t parsed = value.as<int32_t>();
        if (parsed < 0 || parsed > UINT8_MAX)
            return false;
        out = (uint8_t)parsed;
        return true;
    }

    bool readUint32(JsonVariantConst value, uint32_t &out) {
        if (!value.is<uint32_t>())
            return false;
        out = value.as<uint32_t>();
        return true;
    }

    bool readFloat(JsonVariantConst value, float &out) {
        if (!value.is<float>()
                && !value.is<double>()
                && !value.is<int32_t>()
                && !value.is<uint32_t>())
            return false;
        out = value.as<float>();
        return true;
    }

    bool readCString(JsonVariantConst value, char *out, size_t outSize) {
        if (out == nullptr || outSize == 0)
            return false;
        if (!value.is<const char *>())
            return false;
        const char *source = value.as<const char *>();
        if (source == nullptr)
            return false;
        const size_t length = strlen(source);
        if (length >= outSize)
            return false;
        memcpy(out, source, length + 1);
        return true;
    }

    bool readIpAddress(JsonVariantConst value, IPAddress &out) {
        if (!value.is<const char *>())
            return false;
        const char *text = value.as<const char *>();
        if (text == nullptr)
            return false;
        return out.fromString(text);
    }
}
