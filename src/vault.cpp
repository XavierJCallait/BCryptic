#include <sodium.h>
#include <stdexcept>
#include <QByteArray>

#include "Secret.h"
#include "utils.h"
#include "database.h"
#include "vault.h"

Vault::Vault() : vaultKey(32) {
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

void Vault::setupVault(const Secret &masterPassword) {
    if (masterPassword.size() == 0) {
        throw std::runtime_error("Master password must not be empty");
    }

    Secret derived_key(32);
    if (crypto_pwhash(derived_key.data(), derived_key.size(),
                      masterPassword.asChar(), masterPassword.size(),
                      argonParams.salt.data(),
                      argonParams.opslimit, argonParams.memlimit,
                      argonParams.algorithm) != 0) {
        throw std::runtime_error("Failed to derive encryption key, please relaunch app");
    }

    Secret vault_key(32);
    randombytes_buf(vault_key.data(), vault_key.size());

    randombytes_buf(vaultParams.aead_nonce.data(), vaultParams.aead_nonce.size());

    std::vector<unsigned char> associatedData = buildAssociatedData(argonParams.salt, argonParams.opslimit, argonParams.memlimit);

    unsigned long long encrypted_length = vault_key.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES;
    std::vector<unsigned char> encrypted_vk(encrypted_length);

    crypto_aead_xchacha20poly1305_ietf_encrypt(encrypted_vk.data(), &encrypted_length, 
                                              vault_key.data(), vault_key.size(), 
                                              associatedData.data(), associatedData.size(), 
                                              nullptr, 
                                              vaultParams.aead_nonce.data(), derived_key.data());
    
    encrypted_vk.resize(encrypted_length);

    vaultParams.encrypted_vk = std::move(encrypted_vk);

    while (!Database::getInstance().storeVault(argonParams, vaultParams)) {
        continue;
    }
}

void Vault::checkVaultKey(const Secret &masterPassword) {
    if (masterPassword.size() == 0) {
        throw std::runtime_error("Master password must not be empty");
    }

    Secret derived_key(32);
    if (crypto_pwhash(derived_key.data(), derived_key.size(), masterPassword.asChar(), masterPassword.size(), argonParams.salt.data(), argonParams.opslimit, argonParams.memlimit, argonParams.algorithm) != 0 ) {
        throw std::runtime_error("Program error, please relaunch app");
    }

    std::vector<unsigned char> associatedData = buildAssociatedData(argonParams.salt, argonParams.opslimit, argonParams.memlimit);
    
    unsigned long long mlen = 0;
    if (crypto_aead_xchacha20poly1305_ietf_decrypt(vaultKey.data(), &mlen, nullptr, 
                                                   vaultParams.encrypted_vk.data(), vaultParams.encrypted_vk.size(), 
                                                   associatedData.data(), associatedData.size(), 
                                                   vaultParams.aead_nonce.data(), derived_key.data()) != 0) {
        throw std::runtime_error("Wrong password, please try again");
    }
}

std::vector<unsigned char> Vault::encryptItem(const std::vector<unsigned char> &plaintext, const std::vector<unsigned char> &nonce) {
    if (vaultKey.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES) {
        throw std::runtime_error("Vault key not initialized");
    }

    unsigned long long encrypted_length = plaintext.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES;
    std::vector<unsigned char> ciphertext(encrypted_length);

    crypto_aead_xchacha20poly1305_ietf_encrypt(ciphertext.data(), &encrypted_length,
                                               plaintext.data(), plaintext.size(),
                                               nullptr, 0, nullptr, 
                                               nonce.data(), vaultKey.data());

    ciphertext.resize(encrypted_length);

    return ciphertext;
}

std::vector<unsigned char> Vault::decryptItem(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &nonce) {
    if (vaultKey.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES) {
        throw std::runtime_error("Vault key not initialized");
    }

    std::vector<unsigned char> plaintext(ciphertext.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES);

    unsigned long long mlen = 0;
    if (crypto_aead_xchacha20poly1305_ietf_decrypt(plaintext.data(), &mlen, nullptr,
                                                   ciphertext.data(), ciphertext.size(),
                                                   nullptr, 0,
                                                   nonce.data(), vaultKey.data()) != 0) {
        throw std::runtime_error("Wrong vaultKey used, failed to decrypt");
    }

    plaintext.resize(mlen);

    return plaintext;
}

std::string Vault::hashItem(const std::string &item) {
    std::array<char, crypto_pwhash_STRBYTES> hashString{};
    if (crypto_pwhash_str(hashString.data(),
                          item.c_str(), item.size(), 
                          crypto_pwhash_OPSLIMIT_MODERATE,
                          crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
        throw std::runtime_error("Hashing failed");
    }
    return std::string(hashString.data());
}

void Vault::verifyHash(const std::string &storedHash, const std::string &item) {
    if (crypto_pwhash_str_verify(storedHash.c_str(), item.c_str(), item.size()) != 0) {
        throw std::runtime_error("Item does not match hash");
    }
}