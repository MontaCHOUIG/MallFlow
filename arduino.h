#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();
    void sendOpenCommand();

signals:
    void idReceived(const QString &employeeId);

private slots:
    void readSerial();

private:
    QSerialPort *serial;
};

#endif // ARDUINO_H
