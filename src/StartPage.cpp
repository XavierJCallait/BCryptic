#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "StartPage.h"

StartPage::StartPage(QWidget *parent) : QWidget(parent) {
    QLabel *titleLabel = new QLabel("Welcome to BCryptic password manager!", this);
    QLabel *infoLabel = new QLabel("Before you begin, you will need to configure an account. In the following pages, you will be asked some security questions and \nto create a master password. The password will not be stored anywhere and your answers to those questions will be stored securely! \nOnce configured, you will simply need to enter your master password each time you open this up, so don't forget it!", this);
    QLabel *submitLabel = new QLabel("Press the button below to continue", this);
    QFont font = titleLabel->font();
    font.setPointSize(18);
    titleLabel->setFont(font);
    QPushButton *continueButton = new QPushButton("Continue", this);
    continueButton->setFixedHeight(40);
    continueButton->setFixedWidth(200);

    connect(continueButton, &QPushButton::clicked, this, [this]() {
        emit newUserFound();
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(infoLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(submitLabel, 0, Qt::AlignHCenter);
    layout->addWidget(continueButton, 0, Qt::AlignHCenter);
    setLayout(layout);
}
