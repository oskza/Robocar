#include "FactoryReset.h"
#include "platform/esp32/nvs/Nvs.h"
#include "platform/esp32/system/System.h"

namespace Application {
    bool factoryReset() {
        if (!Platform::Nvs::eraseAll())
            return false;
        Platform::System::restart();
    }
}
