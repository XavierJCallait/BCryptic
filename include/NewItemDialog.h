#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <string>

class NewItemDialog : public QDialog {
    Q_OBJECT

public:
    NewItemDialog(std::string item, QWidget *parent = nullptr);
    QString getItem() const;
private:
    QLineEdit *itemLineEdit;
};

#endif