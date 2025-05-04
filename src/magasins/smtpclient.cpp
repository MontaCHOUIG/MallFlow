#include "smtpclient.h"
#include <QDebug>

SmtpClient::SmtpClient(const QString &host, quint16 port, bool useSsl, QObject *parent)
    : QObject(parent), host(host), port(port), ssl(useSsl), socket(nullptr), sslSocket(nullptr), textStream(nullptr)
{
    if (ssl) {
        sslSocket = new QSslSocket(this);
        textStream = new QTextStream(sslSocket);
    } else {
        socket = new QTcpSocket(this);
        textStream = new QTextStream(socket);
    }
    status = NotConnected;
}

SmtpClient::~SmtpClient()
{
    delete textStream;
}

void SmtpClient::setUser(const QString &username)
{
    this->user = username;
}

void SmtpClient::setPassword(const QString &password)
{
    this->pass = password;
}

void SmtpClient::setSenderName(const QString &name)
{
    this->senderName = name;
}

bool SmtpClient::connectToHost()
{
    if (ssl) {
        sslSocket->connectToHostEncrypted(host, port);
        if (!sslSocket->waitForEncrypted(5000)) {
            qDebug() << "SSL connection failed.";
            return false;
        }
    } else {
        socket->connectToHost(host, port);
        if (!socket->waitForConnected(5000)) {
            qDebug() << "Connection failed.";
            return false;
        }
    }

    status = Connected;
    waitForResponse("220");  // SMTP ready
    return true;
}

bool SmtpClient::login()
{
    sendCommand("EHLO " + host + "\r\n");
    waitForResponse("250");

    sendCommand("AUTH LOGIN\r\n");
    waitForResponse("334");

    sendCommand(QString("%1\r\n").arg(QString(user.toUtf8().toBase64())));
    waitForResponse("334");

    sendCommand(QString("%1\r\n").arg(QString(pass.toUtf8().toBase64())));
    if (waitForResponse("235")) {
        status = Authenticated;
        return true;
    }

    return false;
}

bool SmtpClient::sendMail(const MimeMessage &message)
{
    if (status != Authenticated)
        return false;

    sendCommand("MAIL FROM:<" + user + ">\r\n");
    if (!waitForResponse("250")) return false;

    for (const QString &to : message.getRecipients()) {
        sendCommand("RCPT TO:<" + to + ">\r\n");
        if (!waitForResponse("250")) return false;
    }

    sendCommand("DATA\r\n");
    if (!waitForResponse("354")) return false;

    sendCommand(message.toString() + "\r\n.\r\n");
    if (!waitForResponse("250")) return false;

    status = MailSent;
    return true;
}

void SmtpClient::quit()
{
    sendCommand("QUIT\r\n");
    waitForResponse("221");
    status = Quit;
}

void SmtpClient::sendCommand(const QString &cmd)
{
    if (ssl)
        *textStream << cmd;
    else
        *textStream << cmd;

    textStream->flush();
}

bool SmtpClient::waitForResponse(const QString &expectedCode)
{
    QString line;
    while (true) {
        if (ssl) {
            if (!sslSocket->waitForReadyRead(3000)) return false;
            line = sslSocket->readLine();
        } else {
            if (!socket->waitForReadyRead(3000)) return false;
            line = socket->readLine();
        }

        qDebug() << "SMTP Response:" << line.trimmed();
        if (line.startsWith(expectedCode))
            return true;

        if (line[3] != '-')  // end of multiline response
            break;
    }
    return false;
}
