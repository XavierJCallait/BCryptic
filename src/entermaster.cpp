#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "entermaster.h"

EnterMaster::EnterMaster(QWidget *parent) : QWidget(parent) {
    setWindowTitle("BCryptic");
    resize(400, 300);

    QLabel *label = new QLabel("Hello, World!", this);
    QPushButton *button = new QPushButton("Click Me", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);
    setLayout(layout);

    connect(button, &QPushButton::clicked, this, &EnterMaster::onButtonClicked);
}

void EnterMaster::onButtonClicked() {
    
}