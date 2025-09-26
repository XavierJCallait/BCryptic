#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "passwordsw.h"

PasswordsW::PasswordsW(QWidget *parent, std::array<unsigned char, 32> vk) : QMainWindow(parent) {
    qDebug() << vk.data();
}
