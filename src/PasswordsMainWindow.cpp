#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <stdexcept>
#include <sodium.h>

#include "PasswordsMainWindow.h"

PasswordsMainWindow::PasswordsMainWindow(std::shared_ptr<Vault> vault, QWidget *parent) : QMainWindow(parent), vault(std::move(vault)) {

    std::vector<unsigned char> nonce = std::vector<unsigned char> (crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
    randombytes_buf(nonce.data(), nonce.size());
    std::string myString = "Hello, World!";
    std::vector<unsigned char> item(myString.begin(), myString.end());

    try {
        qDebug() << this->vault->encryptItem(item, nonce);
    } catch (std::exception &e) {
        qDebug() << "RIP";
    }
}
