#pragma once

#include <QAbstractListModel>
#include "message.h"

class MessageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MessageRoles {
        SenderRole = Qt::UserRole + 1,
        TypeRole,
        DataSizeRole,
        DataRole,
        TextRole,
        DateTimeRole
    };

    MessageModel(QObject *parent = nullptr);

    void addMessage(const Message &message);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void saveMessages(const QString &filePath);
    void loadMessages(const QString &filePath);

private:
    QList<Message> messages;
};

QDataStream &operator<<(QDataStream &out, const Message &message);
QDataStream &operator>>(QDataStream &in, Message &message);
