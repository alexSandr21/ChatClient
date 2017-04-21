import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{
    id: startWindow
    width: 300
    height: 150

    anchors.centerIn: parent


    radius: 10

    Rectangle
    {
        id:header
        color:"#512A41"

        anchors.fill: parent
        anchors.bottomMargin: 131
        radius: 5

    }

    Rectangle
    {
        id:loginButton
        y: 54
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#512A41"

        height: 27
        width: 150

        radius: 5
        anchors.horizontalCenterOffset: 0

        Text
        {
            anchors.centerIn: parent
            color: "lightgray"
            font.pixelSize: 12
            text:"Log in"
        }

        MouseArea
        {
            anchors.fill:parent
            onPressed:
            {
               loginButton.color =  "#6E3A59"
            }
            onReleased:
            {
                loginButton.color = "#512A41"
                startWindow.visible = false
                loginWindow.visible = true
                loginWindow.logFocus = true
            }
        }

    }
    Rectangle
    {
        id:registrButton
        y: 95
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#512A41"

        height: 27
        width: 150

        radius: 5
        anchors.horizontalCenterOffset: 0

        Text
        {
            anchors.centerIn: parent
            color: "lightgray"
            font.pixelSize: 12
            text:"Registration"
        }

        MouseArea
        {
            anchors.fill:parent
            onPressed:
            {
               registrButton.color =  "#6E3A59"
            }
            onReleased:
            {
                registrButton.color = "#512A41"
                startWindow.visible = false
                regestrationWindow.visible = true
                regestrationWindow.nameFocus = true
            }
        }

    }
}


