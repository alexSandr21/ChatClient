import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


Rectangle
{
    anchors.fill: parent
    color: "gray"
    opacity: 0.5
    visible: false

    MouseArea
    {
        anchors.fill: parent
        onClicked: {}
    }
}
