#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <QSet>
#include <QHash>
#include <QFormLayout>
#include <QLabel>

static constexpr int NUM_QUESTIONS = 10;

class Questions : public QWidget {
    Q_OBJECT

public:
    Questions(QWidget *parent = nullptr);

private slots:
    void updateComboBox(const QString &text, int index);
    
    private:
    QList<QComboBox *> comboBoxes;
    QList<QLineEdit *> answerInputs;
    QHash<QComboBox *, QString> previousSelections;
    QHash<QComboBox *, QLineEdit *> savedAnswers;
    QSet<QString> possibleQuestions;
    void saveQuestionsToDatabase(QLabel *submitLabel);
    void initializeComboBoxes(QFormLayout *form);
};

#endif