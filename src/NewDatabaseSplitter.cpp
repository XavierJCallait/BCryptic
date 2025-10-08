#include <QTreeWidget>
#include <QTableWidget>

#include "NewDatabaseSplitter.h"

NewDatabaseSplitter::NewDatabaseSplitter(QWidget *parent) {
    QTreeWidget *groupTree = new QTreeWidget(this);
    groupTree->setHeaderLabel("WHAT");
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Title", "Password", "URL", "Notes"});
    this->addWidget(groupTree);
    this->addWidget(table);
    this->setStretchFactor(1, 2);
}