#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>

struct Message {
    Message():dateTime(QDateTime::currentDateTime())
    {}

    quint8 sender;
    quint8 type;
    quint16 dataSize;
    qint32 data;
    QString text;
    QDateTime dateTime;
};
