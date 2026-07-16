#include "PreferencesStore.h"

PreferencesStore::PreferencesStore(const char *namespaceStr)
    : _namespaceStr(namespaceStr) {}

bool PreferencesStore::load(const char *key, void *data, size_t size) {
    if (_namespaceStr == nullptr
            || key == nullptr
            || data == nullptr
            || size == 0)
        return false;
    Preferences preferences;
    if (!preferences.begin(_namespaceStr, true))
        return false;
    return preferences.getBytes(key, data, size) == size;
}

bool PreferencesStore::save(const char *key, const void *data, size_t size) {
    if (_namespaceStr == nullptr
            || key == nullptr
            || data == nullptr
            || size == 0)
        return false;
    Preferences preferences;
    if (!preferences.begin(_namespaceStr, false))
        return false;
    return preferences.putBytes(key, data, size) == size;
}

bool PreferencesStore::remove(const char *key) {
    if (_namespaceStr == nullptr || key == nullptr)
        return false;
    Preferences preferences;
    if (!preferences.begin(_namespaceStr, false))
        return false;
    return preferences.remove(key);
}

bool PreferencesStore::clear() {
    if (_namespaceStr == nullptr)
        return false;
    Preferences preferences;
    if (!preferences.begin(_namespaceStr, false))
        return false;
    return preferences.clear();
}
