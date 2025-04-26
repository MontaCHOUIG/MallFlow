#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent) {
    serial = new QSerialPort(this);
    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud9600);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connected on COM3";
    } else {
        qDebug() << "Failed to open serial port:" << serial->errorString();
    }

    connect(serial, &QSerialPort::readyRead, this, &Arduino::readSerial);
}



Arduino::~Arduino() {
    if (serial->isOpen()) {
        serial->close();
    }
}

void Arduino::sendOpenCommand() {
    serial->write("open\n");
}

void Arduino::readSerial() {
    static QByteArray buffer;
    buffer.append(serial->readAll());

    if (buffer.contains('\n')) {
        QString receivedId = QString::fromUtf8(buffer).trimmed();
        buffer.clear();
        emit idReceived(receivedId);
    }
}
