import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{

    property alias logFocus: login.focus
    id:loginWindow
    width: 300
    height: 300

    anchors.centerIn: parent
    radius: 10


    Keys.onEnterPressed:
    {
        loginButton.funcClicked()
    }

    function funcLoginResult(result)
    {
        if(result)
        {
            loginWindow.visible = false
            fade.visible = false
            presenter.slotGetUsers();
        }
        else
        {
            userLogin.border.color = "red"
            userPassword.border.color = "red"
        }

    }

    Rectangle
    {
        id:header
        color:"#512A41"

        anchors.fill: parent
        anchors.bottomMargin: 269
        radius: 5

        Text
        {
            anchors.centerIn: parent
            text:"Log in"
            color: "lightgray"
            font.pixelSize: 16
        }
    }



    Rectangle
    {
        id:loginButton
        y: 207
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#512A41"
        anchors.horizontalCenterOffset: 1

        height: 27
        width: 150

        radius: 5

        Text
        {
            anchors.centerIn: parent
            color: "lightgray"
            font.pixelSize: 12
            text:"Log in"
        }

        MouseArea
        {
            anchors.bottomMargin: 0
            anchors.fill:parent
            onPressed:
            {
               loginButton.color =  "#6E3A59"
            }
            onReleased:
            {
                loginButton.color = "#512A41"
                loginButton.funcClicked()
            }
        }
        function funcClicked()
        {
            userLogin.border.color = "black"
            userPassword.border.color = "black"

            if(login.text!=""&&password.text!="")
            {
                var res = true

                if(!presenter.methCheckString(login.getText(0, login.text.length)))
                {
                    res = false;
                    userLogin.border.color = "red";
                }
                if(!presenter.methCheckString(password.getText(0, password.text.length)))
                {
                    res = false
                    userPassword.border.color = "red"
                }

                if(res)
                    presenter.slotLogin(login.getText(0, login.text.length), password.getText(0, password.text.length));

            }
            else
            {
                if(login.text=="")
                    userLogin.border.color = "red"
                if(password.text =="")
                    userPassword.border.color = "red"
            }
        }
    }

    Rectangle {
        id: backBatton
        x: -1
        y: 246
        width: 150
        height: 27
        color: "#512a41"
        radius: 5
        Text {
            color: "#d3d3d3"
            text: "Back"
            anchors.centerIn: parent
            font.pixelSize: 12
        }

        MouseArea {
            anchors.bottomMargin: 0
            anchors.fill: parent
            onPressed:
            {
               backBatton.color =  "#6E3A59"
            }
            onReleased:
            {
                backBatton.color = "#512A41"
                loginWindow.visible = false
                startWindow.visible = true


                password.text = ""
                login.text = ""

                userLogin.border.color = "black"
                userPassword.border.color = "black"
            }
        }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 1
    }

    Text {
        x: 131
        y: 62
        color: "#000000"
        text: " Login"
        font.pixelSize: 14
        font.family: "Tahoma"
    }

    Rectangle {
        id: userLogin
        x: -8
        y: 85
        width: 150
        height: 24
        color: "#f6e4ef"
        radius: 5

        TextInput
        {
            id: login

            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5

            maximumLength: 17
            Keys.onTabPressed: password.focus = true
            onTextChanged: userLogin.border.color = "black"
        }
        border.width: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#000000"
    }

    Text {
        x: 119
        y: 127
        color: "#000000"
        text: " Password"
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 14
        font.family: "Tahoma"
    }

    Rectangle {
        id: userPassword
        x: 0
        y: 150
        width: 150
        height: 24
        color: "#f6e4ef"
        radius: 5
        TextInput
        {
            id: password

            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5

            maximumLength: 17
            onTextChanged: userPassword.border.color = "black"
            Keys.onTabPressed: login.focus = true
            echoMode: TextInput.Password
        }

        border.width: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#000000"
    }

}
