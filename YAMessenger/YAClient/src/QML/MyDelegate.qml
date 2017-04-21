import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Item {
    width:listView.width
    height:40
    Rectangle
    {
       id: delegRect

       radius: 5
       width: parent.width
       height: parent.height

       color: mainWindow.color
       border.color: "black"
       border.width: 0.5



       MouseArea
       {
           hoverEnabled: true
           anchors.fill:parent
           onEntered: parent.color = "#F6E4EF"
           onExited: parent.color = mainWindow.color

           onClicked:
           {
               if(login!==presenter.methGetReceiver())
               {
                   messageList.friendStatus = name+" "+status
                   messageListModel.clear()
                   messageEnter.clear()
                   newMessage = false
                   presenter.slotSetReceiver(login)
               }
           }
       }

       Text
       {
           text: name
           font.pixelSize: 15
           anchors
           {
               left: parent.left
               verticalCenter: parent.verticalCenter
               leftMargin: 10
           }
       }
       Text
       {
            anchors.margins: 10
            text: status
            font.pixelSize: 12
            color:"gray"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
       }

       Rectangle
       {
           id:newMess
           visible:newMessage
           radius: 5

           anchors
           {
               fill:parent
               topMargin:5
               bottomMargin:parent.height-15
               rightMargin:5
               leftMargin:parent.width-15
           }


           color:"#C20002"

       }

    }



}
