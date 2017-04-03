import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{
    id:connectionWindow
    width: 300
    height: 160

    anchors.centerIn: parent
    radius: 10

    function funcConnect(result)
    {
        if(result==="")
        {
            connectionWindow.visible = false
            startWindow.visible = true
        }
        else
            connectionError.text = result
    }

    Rectangle
    {
        id:header
        color:"#512A41"

        anchors.fill: parent
        anchors.bottomMargin: 131
        radius: 5

        Text
        {
            anchors.centerIn: parent
            text:"Connection"
            color: "lightgray"
            font.pixelSize: 16
        }
    }

    Text
    {
        x: 107
        y: 41

        text:"Enter server IP"
        color: "black"
        font.pixelSize: 13


    }

    Rectangle
    {
        id: serverIP
        y: 63
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#F6E4EF"
        height: 20
        width: 150

        border.color: "black"
        border.width: 0.5
        radius: 5
        anchors.horizontalCenterOffset: 0

        TextEdit
        {
            id:ipServer
            width: serverIP.width
            height: serverIP.height
            clip: true
            anchors.fill: serverIP
            anchors.centerIn: serverIP
            font.pixelSize: 14
            anchors.leftMargin: 5


            onTextChanged:
            {
                //if (text.length>width)

                serverIP.border.color = "black"
                connectionError.text = ""
            }
        }
    }



    Rectangle
    {
        id:connectButton
        y: 108
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#512A41"
        anchors.horizontalCenterOffset: 0

        height: 27
        width: 150

        radius: 5

        Text
        {
            anchors.centerIn: parent
            color: "lightgray"
            font.pixelSize: 12
            text:"Connect"
        }

        MouseArea
        {
            anchors.fill:parent
            onPressed:
            {
               connectButton.color =  "#6E3A59"
            }
            onReleased:
            {
                connectButton.color = "#512A41"

                connectionError.text = ""
                serverIP.border.color = "black"

                if(ipServer.text!="")
                    presenter.slotConnect(ipServer.getText(0, ipServer.text.length))
                else
                   serverIP.border.color = "red"
            }
        }


    }

    Text {
        id: connectionError
        x: 75
        y: 84
        width: 17
        height: 12
        color: "red"
        text: ""
        renderType: Text.NativeRendering
        font.pixelSize: 9
    }



}
