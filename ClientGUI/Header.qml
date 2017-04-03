import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


Rectangle
{
    color:"#512A41"
    radius: 5
    width:listView.width
    height:40

    Text
    {
        font.pixelSize: 15
        color:"lightgray"
        text: "Users"
        anchors.centerIn: parent

    }
    MouseArea
    {
        anchors.fill:parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: presenter.slotGetUsers()
    }
}

