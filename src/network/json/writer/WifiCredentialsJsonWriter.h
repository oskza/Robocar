#ifndef WIFI_CREDENTIALS_JSON_WRITER_H
#define WIFI_CREDENTIALS_JSON_WRITER_H

#include <ArduinoJson.h>
#include "../../WifiCredentials.h"

class WifiCredentialsJsonWriter {
public:
    static void write(JsonObject json, const WifiCredentials &credentials);
};

#endif
