#ifndef STARTUPMAINWINDOWPAGE_H
#define STARTUPMAINWINDOWPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>

#include "vault.h"

enum Indices : int {
    Start = 0,
    SetMaster = 1,
    Questions = 2,
    EnterMaster = 3
};

class StartupMainWindow : public QMainWindow {
    Q_OBJECT

public:
    StartupMainWindow(QWidget *parent = nullptr);

signals:
    void loginSucceeded(std::shared_ptr<Vault> vault);

private:
    QStackedWidget *stack;
};

#endif