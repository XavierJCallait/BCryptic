#ifndef PASSWORDSMAINWINDOW_H
#define PASSWORDSMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <array>

#include "vault.h"

class PasswordsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PasswordsMainWindow(QWidget *parent = nullptr);

    void setVault(std::shared_ptr<Vault> vault) { this->vault = vault; }

signals:

private:
    std::shared_ptr<Vault> vault;
};

#endif