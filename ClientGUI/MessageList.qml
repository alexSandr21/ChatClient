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
    property alias friendStatus: interlocutorStatus.text
    function clear(){messageList.model.clear();messageEnter.clear()}


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

    Text
    {
        id: interlocutorStatus
        color: "lightgray"
        font.pixelSize: 12
        anchors
        {
            left: messageList.left
            bottom: messageList.bottom
            leftMargin: 5
            bottomMargin: 5
        }

        text: "Choose interlocutor"
    }
}

