#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <memory>
#include <vector> 
#include <array>
#include <QByteArray>

#include "Secret.h"

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
    void setupVault(const Secret &masterPassword);
    void checkVaultKey(const Secret &masterPassword);
    std::vector<unsigned char> encryptItem(const std::vector<unsigned char> &plaintext, const std::vector<unsigned char> &nonce);
    std::vector<unsigned char> decryptItem(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &nonce);
    std::string hashItem(const std::string &item);
    void verifyHash(const std::string &storedHash, const std::string &item);
    void verifyDatabaseMAC(const std::string &databaseName);
    void saveDatabaseMAC(const std::string &databaseName);

private:
    void fetchArgonParams();
    QByteArray computeDatabaseMAC(const std::string &databaseName);
    std::vector<unsigned char> buildAssociatedData(const std::vector<unsigned char> &salt, unsigned long long opslimit, size_t memlimit);
    ArgonParams argonParams;
    VaultParams vaultParams;
    Secret vaultKey;
};

#endif