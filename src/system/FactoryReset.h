#ifndef FACTORY_RESET_H
#define FACTORY_RESET_H

namespace System {
    /**
     * Erases all persisted NVS data and restarts the device.
     *
     * Returns false if NVS erasure fails.
     * On success, the function does not return.
     */
    bool factoryReset();
}
#endif
