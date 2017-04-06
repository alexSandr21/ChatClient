import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Window {
    id:mainWindow
    color: "#C1A4BA"
    visible: true
    width: 800
    height: 480
    minimumHeight: 480
    minimumWidth: 800

    title: "YAMessenger"

    Connections
    {
        target: presenter
        onSignalSetUsers: mod.myAppend(name, status, login, false)
        onSignalNewUser: mod.myInsert(name, status, login)
        onSignalConnectResult: connectionWindow.funcConnect(result)
        onSignalLoginResult:loginWindow.funcLoginResult(result)
        onSignalRegistrationResult:regestrationWindow.funcRegistrationResult(result)
        onSignalNewMessage: mod.funcNewMessage(sender)
        onSignalWriteMessage: messageListModel.append({interlocutor:interlocutor, message:message, myanswer:myanswer, time:time})
        onSignalTooBigFile: errorWindow.funcError("file must be not more 30 MB")
        onSignalMessageError: errorWindow.funcError("error send message")
        onSignalSetFriendStatus: mod.funcSetFriendStatus(login)
        onSignalSetTitle: mainWindow.title = "YAMessenger ("+presenter.methGetLogin()+")"
    }

    AttachFileDialog{id:fileDialog}

    MessageEnter{id:messageEnter}

    ChatList{id:chatList; model: MyModel{id:mod}}

    MessageList{id: messageList; model: MessageListModel{id:messageListModel}}

    ButtonSet{id:buttonSet}

    Fade{id:fade; visible: true}

    ConnectionWindow{id:connectionWindow; visible: true}

    StartWindow{id:startWindow; width: 300; height: 160; visible: false}

    LoginWindow{id:loginWindow; visible:false}

    RegistrationWindow{id: regestrationWindow; visible:false}

    ErrorWindow{id:errorWindow; visible: false}

}


