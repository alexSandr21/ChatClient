import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{
    function clear(){input.clear()}
    function getText(){return input.getText(0, input.text.length)}

    id: messageEnter
    height: 70
    width: mainWindow.width-chatList.width
    anchors
    {
        bottom: buttonSet.top
        left:chatList.right
//        fill: parent
//        leftMargin:chatList.width+5
//        rightMargin: 5
//        bottomMargin: parent.height/10
//        topMargin: parent.height*4.5/6+5
    }
    color: "#C1A4BA"
    visible: true

    Rectangle
    {
        id:inputText
        anchors.fill: parent
        anchors.margins: 5
        anchors.bottomMargin: 0

        radius: 5

        color: "#F6E4EF"

        TextArea
        {
            id:input
            anchors.fill: parent
            clip: true
            wrapMode: "Wrap"
        }
    }


}
