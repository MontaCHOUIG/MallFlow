#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "emailsender.h"


class Login : public QDialog {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void onLoginButtonClicked();
    void onCancelButtonClicked();
    void onForgotPasswordClicked();
    void verifySecurityQuestions();


private:
    QLineEdit *emailLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLabel *forgotPasswordLabel;
    QLabel *secureQuestionsLabel;

};

#endif // LOGIN_H
