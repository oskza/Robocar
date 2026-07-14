#include "Nvs.h"
#include <nvs_flash.h>

namespace Platform::Nvs {
    bool eraseAll() {
        return nvs_flash_erase() == ESP_OK
            && nvs_flash_init() == ESP_OK;
    }
}
