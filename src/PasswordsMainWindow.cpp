#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "PasswordsMainWindow.h"

PasswordsMainWindow::PasswordsMainWindow(QWidget *parent) : QMainWindow(parent) {
    std::vector<unsigned char> test = {'a', 'b', 'c'};
    if (vault->encryptItem(test).size() > 0) {
        qDebug() << "WHAT";
    }
}
