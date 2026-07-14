#include "PreferencesStore.h"

bool PreferencesStore::begin(const char *name, bool readOnly) {
    return (name) ? _preferences.begin(name, readOnly) : false;
}

void PreferencesStore::end() { _preferences.end(); }

bool PreferencesStore::load(const char *key, void *data, size_t size) {
    return (key == nullptr || data == nullptr || size == 0)
        ? false
        : _preferences.getBytes(key, data, size) == size;
}

bool PreferencesStore::save(const char *key, const void *data, size_t size) {
    return (key == nullptr || data == nullptr || size == 0)
        ? false
        : _preferences.putBytes(key, data, size) == size;
}

bool PreferencesStore::remove(const char *key) { return (key) ? _preferences.remove(key) : false; }

bool PreferencesStore::clear() { return _preferences.clear(); }
