#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QSettings>
#include <QTextStream>
#include <QFile>

#include "PasswordsMainWindow.h"
#include "StartupMainWindow.h"
#include "database.h"
#include "vault.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream textStream(&styleFile);
        app.setStyleSheet(textStream.readAll());
    } else {
        qWarning() << "Failed to load QSS file: " << styleFile.errorString();
    }

    try {
        Database::getInstance();
    } catch (const std::exception &e) {
        qCritical() << "Failed to initialize database:" << e.what();
        return EXIT_FAILURE;
    }

    StartupMainWindow *startupMainWindow = new StartupMainWindow();
    QObject::connect(startupMainWindow, &StartupMainWindow::loginSucceeded, [](std::shared_ptr<Vault> vault) {
        PasswordsMainWindow *passwordMainWindow = new PasswordsMainWindow(vault);
        passwordMainWindow->show();
    });
    startupMainWindow->show(); 

    // FOR TESTING PURPOSES ONLY
    // Utils::markAsUnconfigured();

    return app.exec();
}