#pragma once

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include "message.h"

class SerialReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    explicit SerialReader(QObject *parent = nullptr, const QString &portName = QString("/dev/ttyUSB0"));
    bool isConnected();

private:
    bool openPort();

signals:
    void newMessage(const Message &message);
    void connectedChanged(bool connected);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void attemptReconnect();

private:
    QSerialPort serialPort;
    QByteArray buffer;
    bool m_connected;
    QTimer reconnectTimer;
    QString portName;
};
