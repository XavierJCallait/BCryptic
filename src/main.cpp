#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QSettings>

#include "entermaster.h"
#include "start.h"
#include "database.h"
#include "utils.h"
#include "vault.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget *mainWidget = new QWidget();

    try {
        Database::getInstance();
    } catch (const std::exception &e) {
        qCritical() << "Failed to initialize database:" << e.what();
        return EXIT_FAILURE;
    }

    if (!Utils::isAppConfigured()) {
        mainWidget = new Start();
    } else {
        mainWidget = new EnterMaster();
    }
  
    mainWidget->show();

    // FOR TESTING PURPOSES ONLY
    Utils::markAsUnconfigured(); 

    return app.exec();
}