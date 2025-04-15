#ifndef SECURITYQUESTION_H
#define SECURITYQUESTION_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

class SecurityQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit SecurityQuestion(QWidget *parent = nullptr);

    QString getQuestion1() const;
    QString getAnswer1() const;
    QString getQuestion2() const;
    QString getAnswer2() const;

private slots:
    void onAccept();

private:
    QComboBox *comboBox1;
    QComboBox *comboBox2;
    QLineEdit *answerLine1;
    QLineEdit *answerLine2;
};

#endif // SECURITYQUESTION_H
