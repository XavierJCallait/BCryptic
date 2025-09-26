#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "passwordsw.h"

PasswordsW::PasswordsW(QWidget *parent, std::array<unsigned char, 32> vk) : QWidget(parent) {
    QLabel *label = new QLabel("Passwords", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    qDebug() << vk.data();
    layout->addWidget(label);
    setLayout(layout);
}
