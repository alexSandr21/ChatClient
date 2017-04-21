import QtQuick 2.0
import QtQuick.Controls 2.1

import QtQuick.Controls 1.4




    Rectangle
    {

        property alias t:txtMess.text

        width: messageList.width*2/3
        height: txtMess.height+7+inter.height+7
        anchors.right: myanswer?parent.right:undefined
        anchors.left: myanswer?undefined:parent.left

        id: messageListDel

        color: myanswer?"#F6E4EF":"white"

        radius: 12

        Text
        {
            id:inter
            clip:true
            text:(myanswer?"I:":interlocutor+":")
            font.pixelSize: 12
            color:"gray"
            anchors
            {
                left:messageListDel.left
                leftMargin:7
                topMargin: 7
            }
        }

        TextArea
        {
            id: txtMess
            text:message
            readOnly: true
            clip: true
            font.pixelSize: 13
            wrapMode: TextEdit.Wrap
            height: contentHeight
            width: messageListDel.width -3
            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            backgroundVisible: false
            frameVisible: false
            textMargin: 3

            x: inter.x
            y: inter.height

        }



        Text
        {
            id:tTime
            text:time
            font.pixelSize: 10
            color: "gray"
            anchors
            {
                fill: messageListDel
                rightMargin: 10
                bottomMargin: 10
                topMargin:messageListDel.height-16
                leftMargin:messageListDel.width-45
            }
        }




}

