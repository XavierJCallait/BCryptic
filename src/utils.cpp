#include <QSettings>
#include <sodium.h>

#include "utils.h"

bool Utils::isAppConfigured() {
    QSettings settings("BCryptic", "BCrypticApp");
    return settings.value("configured", false).toBool();
}

void Utils::markAsConfigured() {
    QSettings settings("BCryptic", "BCrypticApp");
    settings.setValue("configured", true);
}

void Utils::markAsUnconfigured() {
    QSettings settings("BCryptic", "BCrypticApp");
    settings.setValue("configured", false);
}

std::vector<unsigned char> Utils::generateRandomBytes(size_t length) {
    std::vector<unsigned char> buffer(length);
    randombytes_buf(buffer.data(), length);
    return buffer;
}
