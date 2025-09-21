#include <QLabel>
#include <QVBoxLayout>

#include "passwords.h"

Passwords::Passwords(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Passwords", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setLayout(layout);
}
