#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QSettings>

#include "PasswordsMainWindow.h"
#include "StartupMainWindow.h"
#include "database.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        Database::getInstance();
    } catch (const std::exception &e) {
        qCritical() << "Failed to initialize database:" << e.what();
        return EXIT_FAILURE;
    }

    StartupMainWindow *startupMainWindow = new StartupMainWindow();
    PasswordsMainWindow *passwordMainWindow = new PasswordsMainWindow();
    QObject::connect(startupMainWindow, &StartupMainWindow::loginSucceeded, passwordMainWindow, &PasswordsMainWindow::show);
    startupMainWindow->show(); 

    // FOR TESTING PURPOSES ONLY
    // Utils::markAsUnconfigured();

    return app.exec();
}