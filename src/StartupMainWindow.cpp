#include "StartupMainWindow.h"
#include "StartPage.h"
#include "EnterMasterPage.h"
#include "SetMasterPage.h"
#include "QuestionsPage.h"
#include "utils.h"

StartupMainWindow::StartupMainWindow(QWidget *parent) : QMainWindow(parent) {
    stack = new QStackedWidget(this);

    StartPage *start = new StartPage();
    SetMasterPage *setMaster = new SetMasterPage();
    QuestionsPage *questions = new QuestionsPage();
    EnterMasterPage *enterMaster = new EnterMasterPage();

    stack->addWidget(start);
    stack->addWidget(setMaster);
    stack->addWidget(questions);
    stack->addWidget(enterMaster);

    if (Utils::isAppConfigured()) {
        stack->setCurrentIndex(Indices::EnterMaster);
    }

    setCentralWidget(stack);

    connect(start, &StartPage::newUserFound, [this]() {
        stack->setCurrentIndex(Indices::SetMaster);
    });

    connect(setMaster, &SetMasterPage::masterCreated, [this]() {
        stack->setCurrentIndex(Indices::Questions);
    });

    connect(questions, &QuestionsPage::questionsAnswered, [this]() {
        stack->setCurrentIndex(Indices::EnterMaster);
    });

    connect(enterMaster, &EnterMasterPage::loginSucceeded, this, [this](std::shared_ptr<Vault> vault) {
        emit loginSucceeded(vault);
        this->close();
    });
}
