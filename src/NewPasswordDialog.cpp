#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "NewPasswordDialog.h"

NewPasswordDialog::NewPasswordDialog(QSet<QString> titles, QWidget *parent) : QDialog(parent), entryTitles(titles) {
    setModal(true);
    setFixedSize(300, 300);

    QLabel *submitLabel = new QLabel();
    QPushButton *submitButton = new QPushButton("Submit", this);

    titleLineEdit = new QLineEdit(this);
    titleLineEdit->setPlaceholderText(QString("Enter new title"));
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText(QString("Enter new password"));
    confirmPasswordLineEdit = new QLineEdit(this);
    confirmPasswordLineEdit->setPlaceholderText(QString("Confirm new password"));
    urlLineEdit = new QLineEdit(this);
    urlLineEdit->setPlaceholderText(QString("Enter new URL/Purpose"));
    notesLineEdit = new QLineEdit(this);
    notesLineEdit->setPlaceholderText(QString("Enter any notes"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(titleLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(confirmPasswordLineEdit);
    layout->addWidget(urlLineEdit);
    layout->addWidget(notesLineEdit);
    layout->addStretch();
    layout->addWidget(submitLabel);
    layout->addWidget(submitButton);
    
    connect(submitButton, &QPushButton::clicked, this, [this, submitLabel]() {
        if (titleLineEdit->text().isEmpty()) {
            submitLabel->setText("Title cannot be empty");
            return;
        }
        if (passwordLineEdit->text().isEmpty() || confirmPasswordLineEdit->text().isEmpty()) {
            submitLabel->setText("Passwords cannot be empty");
            return;
        }
        if (urlLineEdit->text().isEmpty()) {
            submitLabel->setText("URL/Purpose cannot be empty");
            return;
        }
        if (passwordLineEdit->text() != confirmPasswordLineEdit->text()) {
            submitLabel->setText("Passwords must match before submitting");
            return;
        }
        if (entryTitles.contains(titleLineEdit->text())) {
            submitLabel->setText("Title names must be unique");
            return;
        }
        accept();
    });
}

QString NewPasswordDialog::getTitle() const {
    return titleLineEdit->text();
}

QString NewPasswordDialog::getPassword() const {
    return passwordLineEdit->text();
}

QString NewPasswordDialog::getURL() const {
    return urlLineEdit->text();
}

QString NewPasswordDialog::getNotes() const {
    return notesLineEdit->text();
}