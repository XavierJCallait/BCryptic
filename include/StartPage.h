#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>

class StartPage : public QWidget {
    Q_OBJECT

public:
    StartPage(QWidget *parent = nullptr);

signals:
    void newUserFound();
};

#endif