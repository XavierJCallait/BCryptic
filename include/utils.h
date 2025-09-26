#ifndef UTILS_H
#define UTILS_H

#include <vector>

class Utils {

public:
    static bool isAppConfigured();
    static void markAsConfigured();
    static void markAsUnconfigured();
    static std::vector<unsigned char> generateRandomBytes(size_t length);
};

#endif