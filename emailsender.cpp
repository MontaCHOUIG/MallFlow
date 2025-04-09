#include "emailsender.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>

EmailSender::EmailSender(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &EmailSender::onEmailSent);
}

void EmailSender::sendPasswordResetEmail(const QString &toEmail, const QString &tempPassword) {
    QUrl url("https://api.mailjet.com/v3.1/send");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Replace with your real Mailjet keys
    QString apiKey = "9177b7342a61ced3d8e699b254e5a0a9";
    QString secretKey = "75e6e6b596faef3958f009ef730f1954";

    QString authHeader = "Basic " + QString("%1:%2").arg(apiKey, secretKey).toUtf8().toBase64();
    request.setRawHeader("Authorization", authHeader.toUtf8());

    // Build message content
    QString message = QString(
                          "Hello,\n\n"
                          "We received a request to reset your password for your MallFlow account.\n\n"
                          "Here is your temporary password:\n\n"
                          "   %1\n\n"
                          "Please log in using this password and change it as soon as possible.\n\n"
                          "Best regards,\n"
                          "MallFlow Team"
                          ).arg(tempPassword);

    // Build JSON payload
    QJsonObject messageObj;
    messageObj["From"] = QJsonObject{
        {"Email", "montachouig@gmail.com"},
        {"Name", "MallFlow App"}
    };
    messageObj["To"] = QJsonArray{
        QJsonObject{
            {"Email", toEmail},
            {"Name", toEmail}
        }
    };
    messageObj["Subject"] = "MallFlow - Password Reset Request";
    messageObj["TextPart"] = message;

    QJsonObject body;
    body["Messages"] = QJsonArray{messageObj};

    QJsonDocument doc(body);
    QByteArray jsonData = doc.toJson();

    manager->post(request, jsonData);
}

void EmailSender::onEmailSent(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Email sent successfully!";
    } else {
        qDebug() << "Failed to send email:" << reply->errorString();
    }
    reply->deleteLater();
}
