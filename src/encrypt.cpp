#include "encrypt.h"

std::string Encrypt::encrypt(const std::string& data) {
// Simple XOR encryption for demonstration purposes
    std::string encrypted = data;
    for (char& c : encrypted) {
        c ^= 0xAA; // XOR with a simple key
    }
    return encrypted;
}
std::string Encrypt::decrypt(const std::string& data) {
// Simple XOR decryption (same as encryption)
    return encrypt(data); // XORing again with the same key
}