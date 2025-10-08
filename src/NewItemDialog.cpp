#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "NewItemDialog.h"

NewItemDialog::NewItemDialog(std::string item, QWidget *parent) : QDialog(parent) {
    setModal(true);
    setFixedSize(300, 150);

    QLabel *submitLabel = new QLabel();
    QPushButton *submitButton = new QPushButton("Submit", this);
    QString placeholderText = QString::fromStdString("Enter new " + item + " name");
    itemLineEdit = new QLineEdit(this);
    itemLineEdit->setPlaceholderText(placeholderText);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(itemLineEdit);
    layout->addStretch();
    layout->addWidget(submitLabel);
    layout->addWidget(submitButton);
    
    connect(submitButton, &QPushButton::clicked, this, [this, submitLabel]() {
        if (itemLineEdit->text().isEmpty()) {
            submitLabel->setText("Database name cannot be empty");
            return;
        }
        accept();
    });
}

QString NewItemDialog::getItem() const {
    return itemLineEdit->text();
}