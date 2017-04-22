import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{
    id:errorWindow
    width: 300
    height: 160

    anchors.centerIn: parent
    radius: 10

    Keys.onEnterPressed:
    {
        okButton.funcClicked()
    }

    function funcError(mess)
    {
        fade.visible = true
        errorWindow.visible = true
        errorWindow.focus = true
        errorText.text = mess
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
            text:"Error"
            color: "lightgray"
            font.pixelSize: 16
        }
    }

    Text
    {
        id: errorText
        x: 107
        y: 64

        anchors.horizontalCenter: parent.horizontalCenter
        color: "black"
        font.pixelSize: 13
    }


    Rectangle
    {
        id:okButton
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
            text:"OK"
        }

        MouseArea
        {
            anchors.fill:parent
            onPressed:
            {
               okButton.color =  "#6E3A59"
            }
            onReleased:
            {
                okButton.color = "#512A41"
                 okButton.funcClicked()
            }
        }
        function funcClicked()
        {
            fade.visible = false
            errorWindow.visible = false
            messageEnter.inputFocus = true
        }

    }

}
