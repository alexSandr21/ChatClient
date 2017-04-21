import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


Rectangle
{
    id:chatList
    color: "#C1A4BA"
    width:  200
    height:  parent.height

    property alias mModel: listView.model
    function clear()
    {
        chatList.mModel.clear()
        presenter.slotSetReceiver("")
    }
        ListView
        {
            id: listView

            anchors
            {
                fill: parent
                margins:5
            }

            model:MyModel{}
            header:Header{}
            footer: Footer{}
            delegate:MyDelegate{id:del}

            spacing: 5

        }
    }
