#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QSettings>

#include "EnterMasterPage.h"
#include "StartPage.h"
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
        mainWidget = new StartPage();
    } else {
        mainWidget = new EnterMasterPage();
    }
  
    mainWidget->show();

    // FOR TESTING PURPOSES ONLY
    // Utils::markAsUnconfigured(); 

    return app.exec();
}