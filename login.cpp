#include "login.h"
#include "employe.h"
#include "securityquestion.h"
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>

Login::Login(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login");
    setFixedSize(400, 320); // Bigger window

    // Set lighter purple background
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#d1b3ff")); // Light lavender purple
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

    // "Did you forget your password?" label
    forgotPasswordLabel = new QLabel("<a href='#'>Did you forget your password?</a>");
    forgotPasswordLabel->setTextFormat(Qt::RichText);
    forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotPasswordLabel->setOpenExternalLinks(false);
    forgotPasswordLabel->setAlignment(Qt::AlignCenter);
    forgotPasswordLabel->setVisible(false);

    secureQuestionsLabel = new QLabel("<a href='#'>Use security questions</a>");
    secureQuestionsLabel->setTextFormat(Qt::RichText);
    secureQuestionsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    secureQuestionsLabel->setOpenExternalLinks(false);
    secureQuestionsLabel->setAlignment(Qt::AlignCenter);
    secureQuestionsLabel->setVisible(false);





    QString widgetStyle = R"(
        QLabel {
            font-size: 14px;
            color: #333;
        }
        QLineEdit {
            padding: 10px;
            font-size: 15px;
            border: 1px solid #bbb;
            border-radius: 6px;
            background-color: #ffffff;
        }
        QPushButton {
            padding: 10px;
            font-size: 15px;
            background-color: #a678f0;
            color: white;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #8b55dc;
        }
        QLabel:hover {
            color: #6a0dad;
            text-decoration: underline;
        }
    )";
    setStyleSheet(widgetStyle);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 30, 40, 30);

    layout->addWidget(emailLabel);
    layout->addWidget(emailLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(forgotPasswordLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);

    layout->addSpacing(15);
    layout->addLayout(buttonLayout);
    layout->addWidget(secureQuestionsLabel);
    setLayout(layout);

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &Login::onLoginButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &Login::onCancelButtonClicked);
    connect(forgotPasswordLabel, &QLabel::linkActivated, this, &Login::onForgotPasswordClicked);
    connect(secureQuestionsLabel, &QLabel::linkActivated,this, &Login::verifySecurityQuestions);
}



Login::~Login() {}

void Login::onLoginButtonClicked() {
    QString email = emailLineEdit->text();
    QString password = passwordLineEdit->text();
    Employe e;

    if (e.authenticateUser(email, password)) {
        e.saveAuthenticatedUser(email);
        QMessageBox::information(this, "Login Successful", "Welcome!");
        accept();
    } else {
        // Check if the email exists
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM Employes WHERE email = :email");
        query.bindValue(":email", email);
        if (query.exec() && query.next()) {
            int secureAuth = query.value(0).toInt();
            if (secureAuth == 1) {
                secureQuestionsLabel->setVisible(true); // show link for security questions
            }
            forgotPasswordLabel->setVisible(true); // show forgot password anyway
        } else {
            secureQuestionsLabel->setVisible(false);
            forgotPasswordLabel->setVisible(false);
        }

        QMessageBox::warning(this, "Login Failed", "Invalid information.");
    }
}


void Login::onForgotPasswordClicked() {
    QString email = emailLineEdit->text();
    Employe e ;
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Missing Email", "Please enter your email to reset your password.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT MOT_DE_PASSE FROM Employes WHERE email = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        QString tempPassword = "azerty"; // temporary password
        QString hashedPassword = e.hashPassword(tempPassword);

        // Update the password in the database
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Employes SET MOT_DE_PASSE = :mdp WHERE email = :email");
        updateQuery.bindValue(":mdp", hashedPassword);
        updateQuery.bindValue(":email", email);
        updateQuery.exec();

        // Send email
        EmailSender sender;
        sender.sendPasswordResetEmail(email, tempPassword);

        QMessageBox::information(this, "Password Reset",
                                 "A new temporary password has been sent to your email.");
    } else {
        QMessageBox::warning(this, "Email Not Found",
                             "This email does not exist in our records.");
    }
}

void Login::verifySecurityQuestions() {
    QString email = emailLineEdit->text();
    QSqlQuery query;
    query.prepare("SELECT sq.QUESTION_1, sq.ANSWER_1, sq.QUESTION_2, sq.ANSWER_2 FROM SECURITY_QUESTIONS sq JOIN Employes e ON sq.EMPLOYEE_ID = e.ID_EMPLOYE WHERE e.EMAIL = :email");
    query.bindValue(":email", email);
    if (query.exec() && query.next()) {
        QString q1 = query.value(0).toString();
        QString a1 = query.value(1).toString();
        QString q2 = query.value(2).toString();
        QString a2 = query.value(3).toString();
        if (!query.exec()) {
            QMessageBox::warning(this, "Database Error",
                                 "Query failed: " + query.lastError().text());
            return;
        }

        bool ok;
        QString answer1 = QInputDialog::getText(this, "Security Question 1", q1,
                                                QLineEdit::Normal, "", &ok);
        if (!ok || answer1.trimmed().isEmpty()) return;

        QString answer2 = QInputDialog::getText(this, "Security Question 2", q2,
                                                QLineEdit::Normal, "", &ok);
        if (!ok || answer2.trimmed().isEmpty()) return;

        if (answer1.trimmed().toLower() == a1.trimmed().toLower() &&
            answer2.trimmed().toLower() == a2.trimmed().toLower()) {
            QMessageBox::information(this, "Login Successful", "Welcome!");
            Employe e ;
            e.saveAuthenticatedUser(email);
            accept();
        } else {
            QMessageBox::warning(this, "Incorrect Answers", "Your answers do not match.");
        }

    } else {
        QMessageBox::warning(this, "Error", "Security questions not found.");
    }
}


void Login::onCancelButtonClicked() {
    reject(); // Close login window
}
