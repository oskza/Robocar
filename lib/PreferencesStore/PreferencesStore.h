#ifndef PREFERENCES_STORE_H
#define PREFERENCES_STORE_H
#include <Preferences.h>
#include <stddef.h>

class PreferencesStore {
private:
    Preferences _preferences;
public:
    bool begin(const char *name, bool readOnly = false);
    void end();
    bool load(const char *key, void *data, size_t size);
    bool save(const char *key, const void *data, size_t size);
    bool remove(const char *key);
    bool clear();
};
#endif
