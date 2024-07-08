#include "messagemodel.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void MessageModel::addMessage(const Message &message)
{
    beginInsertRows(QModelIndex(), 0, 0);
    messages.push_front(message);
    endInsertRows();
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return messages.count();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= messages.count())
        return QVariant();

    const Message &message = messages[index.row()];
    if (role == SenderRole)
        return message.sender;
    else if (role == TypeRole)
        return message.type;
    else if (role == DataSizeRole)
        return message.dataSize;
    else if (role == DataRole)
        return QVariant::fromValue(message.data);
    else if (role == TextRole)
        return message.type == 2 ? message.text : QString();
    else if (role == DateTimeRole)
        return  message.dateTime.toString("yy-MM-dd hh:mm:ss.zzz");

    return QVariant();
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SenderRole] = "sender";
    roles[TypeRole] = "type";
    roles[DataSizeRole] = "dataSize";
    roles[DataRole] = "data";
    roles[TextRole] = "text";
    roles[DateTimeRole] = "dateTime";
    return roles;
}

void MessageModel::saveMessages(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_7);
    out << messages;
}

void MessageModel::loadMessages(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file. Creating a new one.");
        saveMessages(filePath);
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);
    QList<Message> loadedMessages;
    in >> loadedMessages;

    beginResetModel();
    messages = loadedMessages;
    endResetModel();
}

QDataStream &operator<<(QDataStream &out, const Message &message)
{
    out << message.sender << message.type << message.dataSize << message.data << message.text << message.dateTime;
    return out;
}

QDataStream &operator>>(QDataStream &in, Message &message)
{
    in >> message.sender >> message.type >> message.dataSize >> message.data >> message.text >> message.dateTime;
    return in;
}
