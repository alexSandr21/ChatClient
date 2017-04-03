import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{
    id: messageList
    color: "#C1A4BA"

    height: mainWindow.height - buttonSet.height-messageEnter.height;
    width: parent.width-chatList.width
    anchors.bottom: messageEnter.top
    anchors.left: chatList.right

    radius: 5
    visible: true
    property alias model: listVeiw.model


    ListView
    {
        anchors.margins: 5
        id:listVeiw
        anchors.fill: parent
        anchors.bottomMargin: messageList.height/7
        model:MessageListModel{id:messageListModel}
        delegate: MessageListDelegate{/*t:message*/}
        spacing: 5


    }
}

