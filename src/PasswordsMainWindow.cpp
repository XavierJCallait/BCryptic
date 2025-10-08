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

#include "NewItemDialog.h"
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
    QAction *newDatabase = new QAction(QIcon(":/icons/NewDatabase.svg"), "New Database", this);
    newDatabase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(newDatabase, &QAction::triggered, this, [this]() {
        openNewDatabaseDialog();
    }); 
    QAction *editDatabase = new QAction("Edit Database", this);
    QAction *deleteDatabase = new QAction("Delete Database", this);
    fileMenu->addAction(newDatabase);
    fileMenu->addAction(editDatabase);
    fileMenu->addAction(deleteDatabase);

    QMenu *categoriesMenu = menuBar->addMenu("Categories");
    QAction *newCategory = new QAction("New Category");
    QAction *editCategory = new QAction("Edit Category");
    QAction *deleteCategory = new QAction("Delete Category");
    categoriesMenu->addAction(newCategory);
    categoriesMenu->addAction(editCategory);
    categoriesMenu->addAction(deleteCategory);
    
    QMenu *entryMenu = menuBar->addMenu("Entry");
    QAction *newEntry = new QAction("New Entry");
    QAction *editEntry = new QAction("Edit Entry");
    QAction *deleteEntry = new QAction("Delete Entry");
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

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->toggleViewAction()->setEnabled(false);
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->addAction("Lock");
    toolBar->addAction("New");
    toolBar->addAction("Edit");
    toolBar->addAction("Delete");
    toolBar->addAction("Share");

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // QSplitter *splitter1 = new QSplitter(this);
    // QTreeWidget *groupTree1 = new QTreeWidget(splitter1);
    // groupTree1->setHeaderLabel("Categories");
    // QTableWidget *table1 = new QTableWidget(splitter1);
    // table1->setColumnCount(4);
    // table1->setHorizontalHeaderLabels({"Title", "Password", "URL", "Notes"});
    // splitter1->addWidget(groupTree1);
    // splitter1->addWidget(table1);
    // splitter1->setStretchFactor(1, 2);

    // QSplitter *splitter2 = new QSplitter(this);
    // QTreeWidget *groupTree2 = new QTreeWidget(splitter2);
    // groupTree2->setHeaderLabel("Groups");
    // QTableWidget *table2 = new QTableWidget(splitter2);
    // table2->setColumnCount(4);
    // table2->setHorizontalHeaderLabels({"Title", "Password", "URL", "Notes"});
    // splitter2->addWidget(groupTree2);
    // splitter2->addWidget(table2);
    // splitter2->setStretchFactor(1, 2);

    // QSplitter *splitter3 = new QSplitter(this);
    // QTreeWidget *groupTree3 = new QTreeWidget(splitter3);
    // groupTree3->setHeaderLabel("Sets");
    // QTableWidget *table3 = new QTableWidget(splitter3);
    // table3->setColumnCount(4);
    // table3->setHorizontalHeaderLabels({"Title", "Password", "URL", "Notes"});
    // splitter3->addWidget(groupTree3);
    // splitter3->addWidget(table3);
    // splitter3->setStretchFactor(1, 2);

    // tabWidget->addTab(splitter1, "Database File 1");
    // tabWidget->addTab(splitter2, "Database File 2");
    // tabWidget->addTab(splitter3, "Database File 3");
}

void PasswordsMainWindow::openNewDatabaseDialog() {
    NewItemDialog newDatabaseDialog = NewItemDialog("database", this);
    if (newDatabaseDialog.exec() == QDialog::Accepted) {
        QString newDatabaseName = newDatabaseDialog.getItem();
        qDebug() << newDatabaseName;
        NewDatabaseSplitter *splitter = new NewDatabaseSplitter();
        tabWidget->addTab(splitter, newDatabaseName);
    }
}