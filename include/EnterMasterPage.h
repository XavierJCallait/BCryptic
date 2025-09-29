#ifndef ENTERMASTERPAGE_H
#define ENTERMASTERPAGE_H

#include <QWidget>

#include "vault.h"

class EnterMasterPage : public QWidget {
    Q_OBJECT

public:
    EnterMasterPage(QWidget *parent = nullptr);

signals:
    void loginSucceeded(std::shared_ptr<Vault> vault);
};

#endif