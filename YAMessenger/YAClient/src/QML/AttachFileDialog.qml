import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2

FileDialog
{
    id:fileDialog

    onAccepted:
    {
        presenter.slotSendFile(fileUrl)
    }
}


