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


private:
    QLineEdit *emailLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLabel *forgotPasswordLabel;
};

#endif // LOGIN_H
