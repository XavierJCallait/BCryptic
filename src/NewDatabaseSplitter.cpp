#include "NewDatabaseSplitter.h"

NewDatabaseSplitter::NewDatabaseSplitter(QWidget *parent) : QSplitter(parent) {
    groupTree = new QTreeWidget(this);
    groupTree->setHeaderLabel("Categories");
    
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Title", "Password", "URL", "Notes"});
    
    this->addWidget(groupTree);
    this->addWidget(table);
    this->setStretchFactor(1, 2);
}