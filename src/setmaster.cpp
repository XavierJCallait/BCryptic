#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "setmaster.h"

SetMaster::SetMaster(QWidget *parent) : QWidget(parent) {
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
        } else if (passwordInput->text() != passwordConfirmation->text()) {
            submitLabel->setText("Passwords do not match, ensure both fields are the same before submitting");
            return;
        } else {
            this->close();
        }
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addWidget(infoLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(passwordInput, 0, Qt::AlignHCenter);
    layout->addWidget(passwordConfirmation, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(submitLabel, 0, Qt::AlignHCenter);
    layout->addWidget(submitButton, 0, Qt::AlignHCenter);

    setLayout(layout);
}