#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

#include "SetMasterPage.h"
#include "utils.h"
#include "Secret.h"
#include "vault.h"

SetMasterPage::SetMasterPage(QWidget *parent) : QWidget(parent) {
    QLabel *titleLabel = new QLabel("Set your Master Password");
    QLabel *infoLabel = new QLabel("You will need to remember this password, as it will be used to access your account\n and encrypt/decrypt your data, the stronger it is, the better! It is recommended to use\n a mix of letters, numbers, and special characters, and size does matter in this case!");
    QLabel *submitLabel = new QLabel();
    QFont font = infoLabel->font();
    font.setPointSize(18);
    titleLabel->setFont(font);
    QPushButton *submitButton = new QPushButton("Submit", this);
    
    submitButton->setFixedHeight(40);
    submitButton->setFixedWidth(200);
    
    QLineEdit *passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter Master Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumWidth(300);
    
    QLineEdit *passwordConfirmation = new QLineEdit(this);
    passwordConfirmation->setPlaceholderText("Confirm Master Password");
    passwordConfirmation->setEchoMode(QLineEdit::Password);
    passwordConfirmation->setMinimumWidth(300);

    connect(submitButton, &QPushButton::clicked, this, [this, passwordInput, passwordConfirmation, submitLabel]() {
        if (passwordInput->text().isEmpty() || passwordConfirmation->text().isEmpty()) {
            submitLabel->setText("Please fill in both fields before submitting");
            return;
        } 
        if (passwordInput->text() != passwordConfirmation->text()) {
            submitLabel->setText("Passwords do not match, ensure both fields are the same before submitting");
            return;
        }
        Secret password(passwordInput->text());
        passwordInput->text().fill(QChar(0));
        passwordInput->text().clear();
        passwordConfirmation->text().fill(QChar(0));
        passwordConfirmation->text().clear();
        try {
            Vault *vault = new Vault();
            vault->setupVault(password);
        } catch (const std::runtime_error &e) {
            submitLabel->setText(e.what());
            return;
        }
        
        Utils::markAsConfigured();
        emit masterCreated();
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(infoLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(passwordInput, 0, Qt::AlignHCenter);
    layout->addWidget(passwordConfirmation, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(submitLabel, 0, Qt::AlignHCenter);
    layout->addWidget(submitButton, 0, Qt::AlignHCenter);

    setLayout(layout);
}