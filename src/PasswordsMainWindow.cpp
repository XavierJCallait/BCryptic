// #include <QLabel>
// #include <QVBoxLayout>
// #include <QDebug>
// #include <stdexcept>
// #include <sodium.h>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QToolBar>
#include <QListWidget>
#include <QTabWidget>
#include <QDialog>
#include <QIcon>
#include <QDockWidget>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <QTreeWidgetItem>

#include "NewItemDialog.h"
#include "database.h"
#include "NewPasswordDialog.h"
#include "NewDatabaseSplitter.h"
#include "PasswordsMainWindow.h"

PasswordsMainWindow::PasswordsMainWindow(std::shared_ptr<Vault> vault, QWidget *parent) : QMainWindow(parent), vault(std::move(vault)) {

    // std::vector<unsigned char> nonce = std::vector<unsigned char> (crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
    // randombytes_buf(nonce.data(), nonce.size());
    // std::string myString = "Hello, World!";
    // std::vector<unsigned char> item(myString.begin(), myString.end());

    // try {
    //     qDebug() << this->vault->encryptItem(item, nonce);
    // } catch (std::exception &e) {
    //     qDebug() << "RIP";
    // }

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("File");

    QAction *newDatabase = new QAction(QIcon(":/resources/icons/NewDatabase.svg"), "New Database", this);
    newDatabase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newDatabase, &QAction::triggered, this, [this]() {
        openNewDatabaseDialog();
    });

    QAction *editDatabase = new QAction("Edit Database", this);
    editDatabase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(editDatabase, &QAction::triggered, this, [this]() {
        openEditDatabaseDialog();
    });

    QAction *deleteDatabase = new QAction("Delete Database", this);
    deleteDatabase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(deleteDatabase, &QAction::triggered, this, [this]() {   
        deleteCurrentDatabase();
    });

    fileMenu->addAction(newDatabase);
    fileMenu->addAction(editDatabase);
    fileMenu->addAction(deleteDatabase);

    QMenu *categoriesMenu = menuBar->addMenu("Categories");

    QAction *newCategory = new QAction("New Category", this);
    newCategory->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    connect(newCategory, &QAction::triggered, this, [this]() {
        openNewCategoryDialog();
    });

    QAction *editCategory = new QAction("Edit Category", this);
    editCategory->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    connect(editCategory, &QAction::triggered, this, [this]() {
        openEditCategoryDialog();
    });

    QAction *deleteCategory = new QAction("Delete Category", this);
    deleteCategory->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_J));
    connect(deleteCategory, &QAction::triggered, this, [this]() {
        deleteCurrentCategory();
    });

    categoriesMenu->addAction(newCategory);
    categoriesMenu->addAction(editCategory);
    categoriesMenu->addAction(deleteCategory);
    
    QMenu *entryMenu = menuBar->addMenu("Entry");

    QAction *newEntry = new QAction("New Entry", this);
    newEntry->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_K));
    connect(newEntry, &QAction::triggered, this, [this]() {
        openNewPasswordDialog();
    });

    QAction *editEntry = new QAction("Edit Entry", this);
    editEntry->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(editEntry, &QAction::triggered, this, [this]() {
        openEditPasswordDialog();
    });

    QAction *deleteEntry = new QAction("Delete Entry", this);
    deleteEntry->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    connect(deleteEntry, &QAction::triggered, this, [this]() {
        deleteCurrentPassword();
    });
    
    entryMenu->addAction(newEntry);
    entryMenu->addAction(editEntry);
    entryMenu->addAction(deleteEntry);

    QMenu *findMenu = menuBar->addMenu("Find");
    QAction *findFile = new QAction("Find File");
    QAction *findCategory = new QAction("Find Category");
    QAction *findEntry = new QAction("Find Entry");
    findMenu->addAction(findFile);
    findMenu->addAction(findCategory);
    findMenu->addAction(findEntry);

    QAction *helpButton = menuBar->addAction("Help");

    fetchDatabase();

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
}

void PasswordsMainWindow::fetchDatabase() {
    Database& database = Database::getInstance();
    std::array<std::string, 3> questions; 
    if (!database.fetchQuestions({6, 9, 10}, questions)) {
        return;
    }
}

void PasswordsMainWindow::openNewDatabaseDialog() {
    NewItemDialog newDatabaseDialog = NewItemDialog("database", this);
    if (newDatabaseDialog.exec() == QDialog::Accepted) {
        QString newDatabaseName = newDatabaseDialog.getItem();
        NewDatabaseSplitter *splitter = new NewDatabaseSplitter();
        tabWidget->addTab(splitter, newDatabaseName);
    }
}

void PasswordsMainWindow::openNewCategoryDialog() {
    NewItemDialog newCategoryDialog = NewItemDialog("category", this);
    if (newCategoryDialog.exec() == QDialog::Accepted) {
        QString newCategoryName = newCategoryDialog.getItem();
        NewDatabaseSplitter *currentSplitter = qobject_cast<NewDatabaseSplitter *>(tabWidget->currentWidget());
        if (!currentSplitter) {
            return;
        }
        databaseTitles[currentSplitter] = QSet<QString>();
        QTreeWidget *groupTree = currentSplitter->getGroupTree();
        QTreeWidgetItem *group = new QTreeWidgetItem(groupTree);
        group->setText(0, newCategoryName);
    }
}

void PasswordsMainWindow::openNewPasswordDialog() {
    NewDatabaseSplitter *currentSplitter = qobject_cast<NewDatabaseSplitter *>(tabWidget->currentWidget());
    if (!currentSplitter) {
        return;
    }
    NewPasswordDialog newPasswordDialog = NewPasswordDialog(databaseTitles[currentSplitter], this);
    if (newPasswordDialog.exec() == QDialog::Accepted) {
        QTableWidget *table = currentSplitter->getTable();
        table->insertRow(TOP_ROW);
        QString title = newPasswordDialog.getTitle();
        databaseTitles[currentSplitter].insert(title);
        table->setItem(TOP_ROW, 0, new QTableWidgetItem(title));
        table->setItem(TOP_ROW, 1, new QTableWidgetItem(newPasswordDialog.getPassword()));
        table->setItem(TOP_ROW, 2, new QTableWidgetItem(newPasswordDialog.getURL()));
        table->setItem(TOP_ROW, 3, new QTableWidgetItem(newPasswordDialog.getNotes()));
    }
}

void PasswordsMainWindow::openEditDatabaseDialog() {

}

void PasswordsMainWindow::deleteCurrentDatabase() {

}

void PasswordsMainWindow::openEditCategoryDialog() {

}

void PasswordsMainWindow::deleteCurrentCategory() {

}

void PasswordsMainWindow::openEditPasswordDialog() {

}

void PasswordsMainWindow::deleteCurrentPassword() {

}