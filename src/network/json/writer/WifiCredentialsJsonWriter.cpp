#include "WifiCredentialsJsonWriter.h"

void WifiCredentialsJsonWriter::write(JsonObject json, const WifiCredentials &credentials) {
    json["ssid"] = credentials.ssid;
    json["password"] = credentials.password;
}
