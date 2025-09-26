#ifndef QUESTIONSPAGE_H
#define QUESTIONSPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <QSet>
#include <QHash>
#include <QFormLayout>
#include <QLabel>

static constexpr int NUM_QUESTIONS = 10;

class QuestionsPage : public QWidget {
    Q_OBJECT

public:
    QuestionsPage(QWidget *parent = nullptr);

private slots:
    void updateComboBox(const QString &text, int index);

signals:
    void questionsAnswered();
    
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