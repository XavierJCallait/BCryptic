#ifndef PASSWORDSW_H
#define PASSWORDSW_H

#include <QMainWindow>
#include <QWidget>
#include <array>

class PasswordsW : public QMainWindow {
    Q_OBJECT

public:
    PasswordsW(QWidget *parent = nullptr, std::array<unsigned char, 32> vk = {});

signals:

private:
    
};

#endif