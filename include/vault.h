#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <memory>
#include <vector> 
#include <array>

struct ArgonParams {
    std::vector<unsigned char> salt;
    unsigned long long opslimit;
    size_t memlimit;
    int algorithm;
};

struct VaultParams {
    std::vector<unsigned char> aead_nonce;
    std::vector<unsigned char> encrypted_vk;
};

class Vault {
public:
    Vault();
    void setupVault(const std::string &masterPassword, std::array<unsigned char, 32> &vk);
    std::array<unsigned char, 32> getVaultKey(const std::string &masterPassword);

private:
    void fetchArgonParams();
    std::vector<unsigned char> buildAssociatedData(const std::vector<unsigned char> &salt, unsigned long long opslimit, size_t memlimit);
    ArgonParams argonParams;
    VaultParams vaultParams;
};

#endif