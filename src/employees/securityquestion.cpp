#include "SecurityQuestion.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

SecurityQuestion::SecurityQuestion(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Sécurité - Questions");

    QLabel *question1Label = new QLabel("Question de sécurité 1:");
    comboBox1 = new QComboBox();
    comboBox1->addItems({
        "Quel est votre film préféré ?",
        "Quel est le prénom de votre mère ?",
        "Dans quelle ville êtes-vous né ?",
        "Quel est le nom de votre premier animal ?"
    });

    QLabel *answer1Label = new QLabel("Réponse:");
    answerLine1 = new QLineEdit();

    QLabel *question2Label = new QLabel("Question de sécurité 2:");
    comboBox2 = new QComboBox();
    comboBox2->addItems({
        "Quel est votre film préféré ?",
        "Quel est le prénom de votre mère ?",
        "Dans quelle ville êtes-vous né ?",
        "Quel est le nom de votre premier animal ?"
    });

    QLabel *answer2Label = new QLabel("Réponse:");
    answerLine2 = new QLineEdit();

    // Buttons
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Annuler");

    connect(okButton, &QPushButton::clicked, this, &SecurityQuestion::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &SecurityQuestion::reject);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(question1Label);
    mainLayout->addWidget(comboBox1);
    mainLayout->addWidget(answer1Label);
    mainLayout->addWidget(answerLine1);

    mainLayout->addWidget(question2Label);
    mainLayout->addWidget(comboBox2);
    mainLayout->addWidget(answer2Label);
    mainLayout->addWidget(answerLine2);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

QString SecurityQuestion::getQuestion1() const {
    return comboBox1->currentText();
}

QString SecurityQuestion::getAnswer1() const {
    return answerLine1->text();
}

QString SecurityQuestion::getQuestion2() const {
    return comboBox2->currentText();
}

QString SecurityQuestion::getAnswer2() const {
    return answerLine2->text();
}

void SecurityQuestion::onAccept() {
    if (comboBox1->currentIndex() == comboBox2->currentIndex()) {
        QMessageBox::warning(this, "Erreur", "Les deux questions doivent être différentes !");
        return;
    }

    if (answerLine1->text().trimmed().isEmpty() || answerLine2->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les réponses ne peuvent pas être vides !");
        return;
    }

    accept();
}
