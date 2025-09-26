#include "StartupMainWindow.h"

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

    setCentralWdiget(stack);

    connect(start, &StartPage::newUserFound, [this]() {
        stack->setCurrentIndex(Indices::SetMaster);
    });

    connect(start, &StartPage::returningUserFound, [this]() {
        stack->setCurrentIndex(Indices::EnterMaster)
    });

    connect(setMaster, &SetMasterPage::masterCreated, [this]() {
        stack->setCurrentIndex(Indices::Questions);
    });

    connect(questions, &QuestionsPage::questionsAnswered, [this]() {
        stack->setCurrentIndex(Indices::EnterMaster);
    });

    connect(enterMaster, &EnterMasterMasterPage::loginSucceeded, this, [this]() {
        emit loginSucceed();
    });
}