#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>

#include "EnterMasterPage.h"
#include "vault.h"

EnterMasterPage::EnterMasterPage(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Enter your master password to access key vault", this);

    QLineEdit *passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter Master Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumWidth(300);
    
    QLabel *submitLabel = new QLabel(this);

    QPushButton *submitButton = new QPushButton("Submit", this);
    submitButton->setFixedHeight(40);
    submitButton->setFixedWidth(200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(label, 0, Qt::AlignHCenter);
    layout->addWidget(passwordInput, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(submitLabel, 0, Qt::AlignHCenter);
    layout->addWidget(submitButton, 0, Qt::AlignHCenter);
    setLayout(layout);

    connect(submitButton, &QPushButton::clicked, this, [this, passwordInput, submitLabel]() {
        QString masterPassword = passwordInput->text();
        if (masterPassword.isEmpty()) {
            submitLabel->setText("Password cannot be empty, please enter your master password"); 
            return;
        }
        std::array<unsigned char, 32> cal_vk = {};
        Vault *vault = new Vault();
        try {
            cal_vk = vault->getVaultKey(passwordInput->text().toStdString());
        } catch (const std::runtime_error &e) {
            submitLabel->setText(e.what());
            return;
        }
        
        emit loginSucceeded();
    });
}