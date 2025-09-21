#include <QSettings>

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
