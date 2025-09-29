#ifndef PASSWORDSMAINWINDOW_H
#define PASSWORDSMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <array>

#include "vault.h"

class PasswordsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PasswordsMainWindow(std::shared_ptr<Vault> vault, QWidget *parent = nullptr);

signals:

private:
    std::shared_ptr<Vault> vault;
};

#endif