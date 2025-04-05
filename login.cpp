#include "login.h"
#include "employe.h"
#include <QMessageBox>
#include <QLabel>

Login::Login(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login");
    setFixedSize(350, 250); // Make the window a bit larger

    // Set solid purple background using palette
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#a678f0")); // Purple background
    setAutoFillBackground(true);
    setPalette(palette);

    // Create UI elements
    QLabel *emailLabel = new QLabel("Email:");
    emailLineEdit = new QLineEdit();

    QLabel *passwordLabel = new QLabel("Password:");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");
    cancelButton = new QPushButton("Cancel");

    // Apply stylesheet to widgets
    QString widgetStyle = R"(
        QLineEdit {
            padding: 6px;
            font-size: 14px;
            border: 1px solid #ccc;
            border-radius: 6px;
            background-color: white;
        }
        QPushButton {
            padding: 8px;
            font-size: 14px;
            background-color: #6a0dad;  /* Darker purple */
            color: white;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #580e9c;
        }

    )";
    setStyleSheet(widgetStyle);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(30, 20, 30, 20);

    layout->addWidget(emailLabel);
    layout->addWidget(emailLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    // Connect buttons
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
