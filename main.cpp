#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "serialreader.h"
#include "messagemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    SerialReader serialReader;
    MessageModel messageModel;

    QObject::connect(&serialReader, &SerialReader::newMessage, &messageModel, &MessageModel::addMessage);


    messageModel.loadMessages("messages.dat");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("messageModel", &messageModel);
    engine.rootContext()->setContextProperty("serialReader", &serialReader);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    int result = app.exec();
    messageModel.saveMessages("messages.dat");

    return result;
}
