#include <sodium.h>

#include "utils.h"

bool Utils::isAppConfigured(const QString &org, const QString &app) {
    QSettings settings(org, app);
    return settings.value("configured", false).toBool();
}

void Utils::markAsConfigured(const QString &org, const QString &app) {
    QSettings settings(org, app);
    settings.setValue("configured", true);
}

void Utils::markAsUnconfigured(const QString &org, const QString &app) {
    QSettings settings(org, app);
    settings.setValue("configured", false);
}

std::vector<unsigned char> Utils::generateRandomBytes(size_t length) {
    std::vector<unsigned char> buffer(length);
    randombytes_buf(buffer.data(), length);
    return buffer;
}
