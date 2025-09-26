#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "PasswordsMainWindow.h"

PasswordsMainWindow::PasswordsMainWindow(QWidget *parent, std::array<unsigned char, 32> vk) : QMainWindow(parent) {
    qDebug() << vk.data();
}
