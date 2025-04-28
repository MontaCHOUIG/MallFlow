#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QString>
#include "mimemessage.h"

// Classe principale pour envoyer un e-mail
class SmtpClient : public QObject
{
    Q_OBJECT

public:
    enum SmtpStatus {
        NotConnected,
        Connected,
        Authenticated,
        MailSent,
        Quit
    };

    explicit SmtpClient(const QString &host, quint16 port = 25, bool useSsl = false, QObject *parent = nullptr);
    ~SmtpClient();

    void setUser(const QString &username);
    void setPassword(const QString &password);
    void setSenderName(const QString &name);

    bool connectToHost();
    bool login();
    bool sendMail(const MimeMessage &message);
    void quit();

private:
    QString host;
    quint16 port;
    bool ssl;
    QString user;
    QString pass;
    QString senderName;

    QTcpSocket *socket;
    QSslSocket *sslSocket;
    QTextStream *textStream;

    SmtpStatus status;

    bool waitForResponse(const QString &expectedCode);
    void sendCommand(const QString &cmd);
};

#endif // SMTPCLIENT_H

