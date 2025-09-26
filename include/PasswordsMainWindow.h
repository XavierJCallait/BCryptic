#ifndef PASSWORDSMAINWINDOW_H
#define PASSWORDSMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <array>

class PasswordsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PasswordsMainWindow(QWidget *parent = nullptr, std::array<unsigned char, 32> vk = {});

signals:

private:
    
};

#endif