#ifndef NEWDATABASESPLITTER_H
#define NEWDATABASESPLITTER_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QSplitter>

class NewDatabaseSplitter : public QSplitter {
    Q_OBJECT

public:
    NewDatabaseSplitter(QWidget *parent = nullptr);

    QTreeWidget *getGroupTree() const { return groupTree; }
    QTableWidget *getTable() const { return table; }

private:
    QTreeWidget *groupTree;
    QTableWidget *table;
};

#endif