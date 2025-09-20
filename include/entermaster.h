#ifndef ENTERMASTER_H
#define ENTERMASTER_H

#include <QWidget>

class EnterMaster : public QWidget {
    Q_OBJECT

    public:
        EnterMaster(QWidget *parent = nullptr);

    private slots:
        void onButtonClicked();
};

#endif