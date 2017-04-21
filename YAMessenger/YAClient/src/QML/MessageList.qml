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
    property alias mModel: listVeiw.model
    property alias friendStatus: interlocutorStatus.text
    function clear(){messageList.mModel.clear();messageEnter.clear()}
    function toBottom(){listVeiw.positionViewAtEnd()}


    ListView
    {
        anchors.margins: 5
        id:listVeiw
        anchors.fill: parent
        anchors.bottomMargin: listVeiw.delegate.height
        model:MessageListModel{id:messageListModel}
        delegate: MessageListDelegate{id:messageListDelegate}
        bottomMargin: 20

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

