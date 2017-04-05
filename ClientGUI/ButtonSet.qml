import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


Rectangle
{
    id:buttonSet
    color: "#C1A4BA"
    height: 50
    width: mainWindow.width - chatList.width
    anchors
    {
        left: chatList.right
        bottom:chatList.bottom
    }

    Rectangle
    {
        id:attachButton

        anchors
        {
            fill: parent
            rightMargin:5
            topMargin:5
            bottomMargin:5
            leftMargin: parent.width*9/10
        }

        radius: 5

        color:"#512A41"

        Text
        {
            font.pixelSize: 12
            anchors.centerIn: parent
            text:"Attach"
            color: "lightgray"

            MouseArea
            {
                anchors.fill: parent
                onPressed:
                {
                   attachButton.color =  "#6E3A59"
                }
                onReleased:
                {
                    attachButton.color = "#512A41"

                    if(presenter.methIsOnline(presenter.methGetReceiver()))
                        fileDialog.visible = true;
                }
            }
        }

    }

    Rectangle
    {
        id:sendButton

        anchors
        {
            fill: parent
            rightMargin: parent.width/10+5
            leftMargin:5
            topMargin:5
            bottomMargin:5
        }

        radius: 5

        color:"#512A41"

        Text
        {
            font.pixelSize: 12
            anchors.centerIn: parent
            text:"Send"
            color: "lightgray"
        }
        MouseArea
        {
            anchors.fill:parent

            onPressed:
            {
               sendButton.color =  "#6E3A59"
            }
            onReleased:
            {
                sendButton.color = "#512A41"
                if(messageEnter.getText()!==""&&presenter.methIsOnline(presenter.methGetReceiver()))
                {
                    presenter.slotSendMessage(messageEnter.getText())
                    messageEnter.clear();
                }
            }
        }

    }
}
