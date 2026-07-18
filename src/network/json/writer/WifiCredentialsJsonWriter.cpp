#include "WifiCredentialsJsonWriter.h"

namespace WifiCredentialsJsonWriter {
    void write(JsonObject json, const WifiCredentials &credentials) {
        json["ssid"] = credentials.ssid;
        json["password"] = credentials.password;
    }
}
