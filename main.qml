import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Message Viewer")

    Rectangle {
        id: connectionStatus
        width: parent.width
        height: 40
        color: serialReader.connected ? "green" : "red"
        Text {
            anchors.centerIn: parent
            text: serialReader.connected ? "Connected" : "Disconnected"
            color: "white"
            font.bold: true
        }
    }

    // Таблица сообщений
    ListView {
        id: listView
        anchors.top: connectionStatus.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: messageModel
        clip: true

        delegate: Row {
            width: listView.width
            height: 40
            spacing: 2
            Rectangle {
                width: 180
                height: parent.height
                color: index % 2 == 0 ? "lightgray" : "white"
                Text { text: model.dateTime; anchors.centerIn: parent }
            }
            Rectangle {
                width: 100
                height: parent.height
                color: index % 2 == 0 ? "lightgray" : "white"
                Text { text: model.sender; anchors.centerIn: parent }
            }
            Rectangle {
                width: 100
                height: parent.height
                color: index % 2 == 0 ? "lightgray" : "white"
                Text { text: model.type; anchors.centerIn: parent }
            }
            Rectangle {
                width: 100
                height: parent.height
                color: index % 2 == 0 ? "lightgray" : "white"
                Text { text: model.dataSize; anchors.centerIn: parent }
            }
            Rectangle {
                width: 200
                height: parent.height
                color: index % 2 == 0 ? "lightgray" : "white"
                Text { text: model.type == 1 ? model.data : model.text ; anchors.centerIn: parent }
            }

        }
    }
}
