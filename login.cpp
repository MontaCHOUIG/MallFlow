#include "login.h"
#include "employe.h"
#include <QMessageBox>
#include <QLabel>

Login::Login(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login");

    // Create UI elements
    QLabel *emailLabel = new QLabel("Email:");
    emailLineEdit = new QLineEdit();

    QLabel *passwordLabel = new QLabel("Password:");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Hide password input

    loginButton = new QPushButton("Login");
    cancelButton = new QPushButton("Cancel");

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(emailLabel);
    layout->addWidget(emailLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(cancelButton);

    setLayout(layout);

    // Connect buttons to functions
    connect(loginButton, &QPushButton::clicked, this, &Login::onLoginButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &Login::onCancelButtonClicked);
}

Login::~Login() {}

void Login::onLoginButtonClicked() {
    QString email = emailLineEdit->text();
    QString password = passwordLineEdit->text();
    Employe e;
    if (e.authenticateUser(email, password)) {
        e.saveAuthenticatedUser(email);
        QMessageBox::information(this, "Login Successful", "Welcome!");
        accept(); // Close login window and return success
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid email or password.");
    }
}

void Login::onCancelButtonClicked() {
    reject(); // Close login window
}
