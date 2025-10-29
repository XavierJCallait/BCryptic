#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <string>
#include <QSet>

class NewPasswordDialog : public QDialog {
    Q_OBJECT

public:
    NewPasswordDialog(QSet<QString> titles, QWidget *parent = nullptr);
    QString getTitle() const;
    QString getPassword() const;
    QString getURL() const;
    QString getNotes() const;
private:
    QSet<QString> entryTitles;
    QLineEdit *titleLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QLineEdit *urlLineEdit;
    QLineEdit *notesLineEdit;
};

#endif