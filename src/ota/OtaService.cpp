#include "OtaService.h"
#include <ArduinoOTA.h>

OtaService::OtaService() : _started(false) {}

bool OtaService::begin(const char *hostname) {
    if (_started)
        return true;
    if (hostname == nullptr || hostname[0] == '\0')
        return false;
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.begin();
    _started = true;
    return true;
}

void OtaService::update() {
    if (_started)
        ArduinoOTA.handle();
}

bool OtaService::isStarted() const { return _started; }
