#include <sodium.h>
#include <stdexcept>
#include <QDebug>
#include <QByteArray>

#include "utils.h"
#include "database.h"
#include "vault.h"

Vault::Vault() {
    if (!Utils::isAppConfigured()) {
        if (sodium_init() < 0) {
            throw std::runtime_error("Failed to initialize libsodium");
        }
    }
    fetchArgonParams();
}   

void Vault::fetchArgonParams() {
    if (Utils::isAppConfigured()) {
        if (!Database::getInstance().fetchVault(argonParams, vaultParams)) {
            throw std::runtime_error("Failed to retrieve vault");
        }
    } else {
        argonParams.salt = Utils::generateRandomBytes(crypto_pwhash_SALTBYTES);
        argonParams.opslimit = crypto_pwhash_OPSLIMIT_MODERATE;
        argonParams.memlimit = crypto_pwhash_MEMLIMIT_MODERATE;
        argonParams.algorithm = crypto_pwhash_ALG_ARGON2ID13;
        vaultParams.aead_nonce = Utils::generateRandomBytes(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
        vaultParams.encrypted_vk = std::vector<unsigned char>(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);
    }
}

std::vector<unsigned char> Vault::buildAssociatedData(const std::vector<unsigned char> &salt, unsigned long long opslimit, size_t memlimit) {
    std::vector<unsigned char> associatedData;
    associatedData.insert(associatedData.end(), salt.begin(), salt.end());
    for (int i = 0; i < 8; ++i) {
        associatedData.push_back((opslimit >> (i*8)) & 0xFF);
    }
    for (int i = 0; i < 8; ++i) {
        associatedData.push_back(((unsigned long long)memlimit >> (i*8)) & 0xFF);
    }
    return associatedData;
}

void Vault::setupVault(const std::string &masterPassword, std::array<unsigned char, 32> &vk) {
    if (masterPassword.empty()) {
        throw std::runtime_error("No master password");
    }

    std::array<unsigned char, 32> derived_key{}; // determine size
    if (crypto_pwhash(derived_key.data(), derived_key.size(),
                      masterPassword.c_str(), masterPassword.size(),
                      argonParams.salt.data(),
                      argonParams.opslimit, argonParams.memlimit,
                      argonParams.algorithm) != 0) {
        throw std::runtime_error("Failed to derive encryption key");
    }

    randombytes_buf(vk.data(), vk.size());

    randombytes_buf(vaultParams.aead_nonce.data(), vaultParams.aead_nonce.size());

    std::vector<unsigned char> associatedData = buildAssociatedData(argonParams.salt, argonParams.opslimit, argonParams.memlimit);

    unsigned long long encrypted_length = vk.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES;
    std::vector<unsigned char> encrypted_vk(encrypted_length);

    crypto_aead_xchacha20poly1305_ietf_encrypt(encrypted_vk.data(), &encrypted_length, 
                                              vk.data(), vk.size(), 
                                              associatedData.data(), associatedData.size(), 
                                              nullptr, 
                                              vaultParams.aead_nonce.data(), derived_key.data());
    
    encrypted_vk.resize(encrypted_length);

    vaultParams.encrypted_vk = std::move(encrypted_vk);

    Database::getInstance().storeVault(argonParams, vaultParams);
}

std::array<unsigned char, 32> Vault::getVaultKey(const std::string &masterPassword) {
    if (masterPassword.empty()) {
        throw std::runtime_error("No master password");
    }
    std::array<unsigned char, 32> derived_key{};
    if (crypto_pwhash(derived_key.data(), derived_key.size(), masterPassword.c_str(), masterPassword.size(), argonParams.salt.data(), argonParams.opslimit, argonParams.memlimit, argonParams.algorithm) != 0 ) {
        throw std::runtime_error("Failed to derive decryption key");
    }
    std::vector<unsigned char> associatedData = buildAssociatedData(argonParams.salt, argonParams.opslimit, argonParams.memlimit);
    std::array<unsigned char, 32> vk{};
    unsigned long long mlen = 0;
    if (crypto_aead_xchacha20poly1305_ietf_decrypt(vk.data(), &mlen, nullptr, vaultParams.encrypted_vk.data(), vaultParams.encrypted_vk.size(), associatedData.data(), associatedData.size(), vaultParams.aead_nonce.data(), derived_key.data()) != 0) {
        throw std::runtime_error("Failed to retrieve vault key");
    }
    return vk;
}