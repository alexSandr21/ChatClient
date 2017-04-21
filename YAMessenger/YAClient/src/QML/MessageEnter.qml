import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

import QtQuick.Controls 1.4



Rectangle
{
    property alias text: input.text
    function clear(){input.text = ""}
    function getText(){return input.getText(0, input.text.length)}

    id: messageEnter
    height: 70
    width: mainWindow.width-chatList.width
    anchors
    {
        bottom: buttonSet.top
        left:chatList.right
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
            verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
            id:input
            anchors.fill: parent
            clip: true
            wrapMode: TextEdit.Wrap
            backgroundVisible: false
            frameVisible: false

            anchors.margins: 2


            Keys.onEnterPressed:
            {
                buttonSet.funcSend()
            }

        }
    }
}







