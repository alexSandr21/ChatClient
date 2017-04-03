import QtQuick 2.0




    Rectangle
    {

        property alias t:txtMess.text

        width: messageList.width*2/3
        height: messageList.height/7
        //height: txtMess.height
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
        Rectangle
        {
            width: messageListDel.width-20
            height: txtMess.height
            color: messageListDel.color
            anchors
            {
                top: inter.bottom
                bottom:tTime.top
                left:messageListDel.left
                leftMargin: 8
            }

            Text
            {
                id: txtMess
                text:message
                clip: true
                font.pixelSize: 13
                anchors.fill:parent
                wrapMode: "Wrap"

            }
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




   // }
}

