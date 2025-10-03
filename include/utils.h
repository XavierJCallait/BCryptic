#ifndef UTILS_H
#define UTILS_H

#include <QSettings>
#include <vector>

class Utils {

public:
    static bool isAppConfigured(const QString &org = "BCryptic", const QString &app = "BCrypticApp");
    static void markAsConfigured(const QString &org = "BCryptic", const QString &app = "BCrypticApp");
    static void markAsUnconfigured(const QString &org = "BCryptic", const QString &app = "BCrypticApp");
    static std::vector<unsigned char> generateRandomBytes(size_t length);
};

#endif