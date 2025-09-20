#include <QStringList>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <list>
#include <string>
#include <utility>

#include "setmaster.h"
#include "database.h"
#include "questions.h"

Questions::Questions(QWidget *parent) : QWidget(parent) {
    QStringList questionList = {
      "What was the first concert you attended?",
      "What place did your parents meet?",
      "What was the name of your favorite school teacher?",
      "What was a childhood nickname of yours?",
      "What exam did you do the worst on?",
      "What is the name of school you wanted to go to but didn't attend?",
      "What was the name of your first secret crush?",
      "What street was the street name of your first childhood home?",
      "What was the name of your favorite coworker?",
      "What food did you hate as a child but enjoy now?",
      "What was the first movie you saw in a theater?",
      "What was the most embarassing moment you had?",
      "What was the first thing you bought with your own money?",
      "What is your favorite smell?",
      "What career did you wanted at some point but never pursued?"
    };
    possibleQuestions = QSet<QString>(questionList.begin(), questionList.end());
    QWidget *formWidget = new QWidget(this);
    QFormLayout *form = new QFormLayout(formWidget);

    // Initialize combo boxes and line edits
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        QComboBox *comboBox = new QComboBox(this);
        comboBox->setPlaceholderText("Select a question");
        comboBox->addItems(possibleQuestions.values());

        QLineEdit *answerInput = new QLineEdit(this);
        answerInput->setPlaceholderText("Enter answer here");

        previousSelections[comboBox] = QString();
        comboBoxes.append(comboBox);
        answerInputs.append(answerInput);
        form->addRow(comboBox, answerInput);
        connect(comboBox, &QComboBox::currentTextChanged, this, [this, i](const QString &question) {
            if (savedAnswers.contains(comboBoxes[i])) {
                answerInputs[i]->clear();
                savedAnswers.remove(comboBoxes[i]);
            }
            updateComboBox(question, i);
        });
        connect(answerInput, &QLineEdit::textChanged, this, [this, i]() {
            QString question = comboBoxes[i]->currentText();
            if (!question.isEmpty()) {
                savedAnswers[comboBoxes[i]] = answerInputs[i];
            }
        });
    }
    QLabel *infoLabel = new QLabel("Please select security questions from the drop downs and answer the following questions: ");
    QLabel *submitLabel = new QLabel();
    QPushButton *submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, [this, submitLabel]() {
        saveQuestionsToDatabase(submitLabel);
    });

    formWidget->setMinimumWidth(600);
    formWidget->setMaximumWidth(1600);
    submitButton->setFixedHeight(40);
    submitButton->setFixedWidth(200);

    // Horizontal center form
    QHBoxLayout *formRow = new QHBoxLayout();
    formRow->addStretch();
    formRow->addWidget(formWidget);
    formRow->addStretch();

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(infoLabel, 0, Qt::AlignHCenter);
    outerLayout->addStretch();
    outerLayout->addLayout(formRow);
    outerLayout->addStretch();
    outerLayout->addWidget(submitLabel, 0, Qt::AlignHCenter);
    outerLayout->addWidget(submitButton, 0, Qt::AlignHCenter);

    setLayout(outerLayout);
}

void Questions::updateComboBox(const QString &text, int index) {
    QComboBox *changedComboBox = comboBoxes[index];
    QString previousText = previousSelections[changedComboBox];
    
    if (!previousText.isEmpty()) {
        possibleQuestions.insert(previousText);
    }
    previousSelections[changedComboBox] = text;
    possibleQuestions.remove(text);

    // Update options for other combo boxes
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (i == index) {
            continue;
        }
        QComboBox *comboBox = comboBoxes[i];
        QString currentSelection = previousSelections[comboBox];
        comboBox->blockSignals(true);
        comboBox->clear();
        QStringList availableQuestions = possibleQuestions.values();
        if (!currentSelection.isEmpty()) {
            availableQuestions.prepend(currentSelection);
        }
        comboBox->addItems(availableQuestions);
        comboBox->setCurrentText(currentSelection);
        comboBox->blockSignals(false);
    }
}

void Questions::saveQuestionsToDatabase(QLabel *submitLabel) {
    if (savedAnswers.size() != NUM_QUESTIONS) {
        submitLabel->setText("Please select and answer all questions before submitting");
        return;
    }
    std::list<std::pair<std::string, std::string>> question_answer_list;
    for (auto iter = savedAnswers.begin(); iter != savedAnswers.end(); ++iter) {
        QComboBox *comboBox = iter.key();
        QLineEdit *lineEdit = iter.value();
        std::string question = comboBox->currentText().toStdString();
        std::string answer = lineEdit->text().toStdString();
        question_answer_list.push_back(std::make_pair(question, answer));
    }
    Database& database = Database::getInstance();
    database.storeQuestions(question_answer_list);
    SetMaster *page = new SetMaster();
    page->show();
    this->close();
}