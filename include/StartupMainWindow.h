#ifndef STARTUPMAINWINDOWPAGE_H
#define STARTUPMAINWINDOWPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>

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
    void loginSucceeded();

private:
    QStackedWidget *stack;
};

#endif