#ifndef PASSWORDSW_H
#define PASSWORDSW_H

#include <QWidget>
#include <array>

class PasswordsW : public QWidget {
    Q_OBJECT

    public:
        PasswordsW(QWidget *parent = nullptr, std::array<unsigned char, 32> vk = {});
};

#endif