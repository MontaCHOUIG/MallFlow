#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class EmailSender : public QObject {
    Q_OBJECT

public:
    explicit EmailSender(QObject *parent = nullptr);
    void sendPasswordResetEmail(const QString &toEmail, const QString &message);

private slots:
    void onEmailSent(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // EMAILSENDER_H
