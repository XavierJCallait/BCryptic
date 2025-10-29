#ifndef PASSWORDSMAINWINDOW_H
#define PASSWORDSMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <array>
#include <QHash>
#include <QSet>

#include "NewDatabaseSplitter.h"
#include "vault.h"

#define TOP_ROW 0

class PasswordsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PasswordsMainWindow(std::shared_ptr<Vault> vault, QWidget *parent = nullptr);

signals:

private:
    std::shared_ptr<Vault> vault;
    QTabWidget *tabWidget;
    QHash<NewDatabaseSplitter *, QSet<QString>> databaseTitles;
    void openNewDatabaseDialog();
    void openEditDatabaseDialog();
    void deleteCurrentDatabase();

    void openNewCategoryDialog();
    void openEditCategoryDialog();
    void deleteCurrentCategory();

    void openNewPasswordDialog();
    void openEditPasswordDialog();
    void deleteCurrentPassword(); 

    void fetchDatabase();
};

#endif