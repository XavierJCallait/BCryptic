#ifndef ENCRYPT_H

#define ENCRYPT_H

#include <string>
#include <vector>

class Encrypt {
    public:
        static std::string encrypt(const std::string& data);
        static std::string decrypt(const std::string& data);
};

#endif