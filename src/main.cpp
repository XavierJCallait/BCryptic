#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QSettings>

#include "setmaster.h"
#include "questions.h"
#include "database.h"

bool hasBeenConfigured() {
    QSettings settings("BCryptic", "BCrypticApp");
    return settings.value("configured", false).toBool();
}

void markAsConfigured() {
    QSettings settings("BCryptic", "BCrypticApp");
    settings.setValue("configured", true);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget *mainWidget = new QWidget();

    try {
        Database::getInstance();
    } catch (const std::exception &e) {
        qCritical() << "Failed to initialize database:" << e.what();
        return EXIT_FAILURE;
    }

    if (!hasBeenConfigured()) {
        // mainWidget = new SetMaster();
        mainWidget = new Questions();
        markAsConfigured();
    }
  
    mainWidget->show();


    // FOR DEV TESTING PURPOSES ONLY
    QSettings settings("BCryptic", "BCrypticApp");
    settings.setValue("configured", false);
    // END DEV TESTING PURPOSES ONLY


    return app.exec();
}