#include "serialreader.h"
#include <QDataStream>
#include <QDebug>

SerialReader::SerialReader(QObject *parent, const QString &portName)
    : QObject(parent), m_connected(false), portName(portName)
{
    connect(&serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&reconnectTimer, &QTimer::timeout, this, &SerialReader::attemptReconnect);
    reconnectTimer.setInterval(1000);
    reconnectTimer.start();
}

bool SerialReader::openPort()
{
    serialPort.setPortName(portName);
    serialPort.setBaudRate(QSerialPort::Baud115200);

    if(serialPort.isOpen())
        serialPort.close();

    if (!serialPort.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open serial port:" << serialPort.errorString();
        m_connected = false;
        emit connectedChanged(m_connected);
        return false;
    }

    connect(&serialPort, &QSerialPort::readyRead, this, &SerialReader::readData);
    m_connected = true;
    emit connectedChanged(m_connected);
    return true;
}

bool SerialReader::isConnected()
{
    auto result = serialPort.isOpen() && serialPort.isReadable() && serialPort.error() == QSerialPort::NoError;
    if(m_connected != result)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
    return  result;
}

void SerialReader::readData()
{
    if (!isConnected()) {
        qWarning() << "Serial port not open";
        return;
    }

    buffer.append(serialPort.readAll());

//    while (buffer.size() >= 6) {
        QDataStream stream(buffer);
        stream.setByteOrder(QDataStream::LittleEndian);

        quint8 sender;
        quint8 type;
        quint16 dataSize;
        stream >> sender >> type >> dataSize;

        int messageSize = 6 + dataSize + 2;

//        if (buffer.size() < messageSize) {
//            return;
//        }

        QByteArray messageData = buffer.mid(4, dataSize);
        quint16 crc;
        stream >> crc;

        // TODO: Проверить CRC

        Message message;
        message.sender = sender;
        message.type = type;
        message.dataSize = dataSize;

        if (type == 1) {
            QDataStream dataStream(messageData);
            dataStream.setByteOrder(QDataStream::LittleEndian);
//            while (!dataStream.atEnd()) {
                qint32 value;
                dataStream >> value;
                message.data = value;
//            }
        } else if (type == 2) {
            message.text = QString::fromUtf8(messageData);
        }

        qDebug() << "Received message:";
        qDebug() << "Sender:" << message.sender;
        qDebug() << "Type:" << message.type;
        qDebug() << "Data Size:" << message.dataSize;
        if (message.type == 1) {
            qDebug() << "Data:" << message.data;
        } else if (message.type == 2) {
            qDebug() << "Text:" << message.text;
        }

        emit newMessage(message);
        buffer.remove(0, messageSize);
//    }
}

void SerialReader::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError || error == QSerialPort::PermissionError || error == QSerialPort::OpenError) {
        qWarning() << "Serial port error:" << serialPort.errorString();
        serialPort.reset();
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void SerialReader::attemptReconnect()
{
    if (!isConnected()) {
            qDebug() << "Attempting to reconnect...";
            openPort();
        }
}
